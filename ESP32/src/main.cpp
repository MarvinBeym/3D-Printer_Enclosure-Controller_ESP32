#include <Arduino.h>

#include <Preferences.h>
#include <Adafruit_Sensor.h>
#include <DHT.h>
#include <Wire.h>
#include <SPI.h>
#include <WiFi.h>
#include <DNSServer.h>
#include <WebServer.h>
#include <WiFiManager.h> 
#include <ESPUI.h>

#define FASTLED_ALLOW_INTERRUPTS 0

#include <FastLED.h>

#include "Relay.h"
#include "PinDefinition.h"
#include "Configuration.h"
#include "Definitions.h"

#include <Nextion.h>
#define nextion Serial2



const String esp32_version = "1.3.1";
const String display_version = "1.3.1";

//Storage
Preferences preferences;

//Watcher
bool temperature_warnState = false;
bool temperature_dangState = false;
int temperature_warnThreshold = 40;
int temperature_dangThreshold = 60;
unsigned long temperature_warner_previousMillis = 0;
bool normalState = true;

//WIFI
const byte DNS_PORT = 53;
IPAddress apIP(192, 168, 1, 1);
DNSServer dnsServer;
WiFiManager wifiManager;

//Dual core
TaskHandle_t Led2EffectsTask;
TaskHandle_t SensorTask;
//Temperature and humidity sensor
DHT dht_1(dht22_1_pin, DHT22);
DHT dht_2(dht22_2_pin, DHT22);
unsigned long dht_previous_millis = 0;
float dht_1_humidity = 0;
float dht_1_temperature = 0;
float send_dht_1_humidity;
float send_dht_1_temperature;
float dht_2_humidity = 0;
float dht_2_temperature = 0;
float send_dht_2_humidity;
float send_dht_2_temperature;

//Fan Control
unsigned int fan1rpm = 0;
unsigned int send_fan1rpm;
unsigned long timeoldFan1;
volatile int half_revolutionsFan1;
int fan1_dutyCycle;

unsigned int fan2rpm = 0;
unsigned int send_fan2rpm;
unsigned long timeoldFan2;
volatile int half_revolutionsFan2;
int fan2_dutyCycle;

//Relays
Relay led1_relay(relay_in_pin, false);

//LED2
CRGB leds[led2_numberOfLEDs];
int led2_mode_selected = 0;
String led2_mode = "solid";
int led2_numberOfModes = 6;
int led2_color_selected = 0;
int led2_effect_brightness = 0;
int led2_effect_fadeAmount = 1;
int led2_effect_counter1 = 0;
int led2_effect_counter2 = led2_numberOfLEDs;
int led2_brightness = 80;
unsigned long led2_effect_previousMillis = 0;
//Display
Nextion myNextion(nextion, 115200);

unsigned long display_previous_millis = 0;
int currentPage = MAIN_PAGE;
bool display_sleepState = 0;
int display_brightness = 80;

//ESPUI
uint16_t espui_sens1Graph_compID;
uint16_t espui_sens2Graph_compID;
uint16_t espui_messageBox_compID;
uint16_t espui_led1Control_compID;
uint16_t espui_led2ColorSelect_compID;
uint16_t espui_led2ModeSelect_compID;
uint16_t espui_sensor1_temp_compID;
uint16_t espui_sensor1_humi_compID;
uint16_t espui_sensor2_temp_compID;
uint16_t espui_sensor2_humi_compID;
uint16_t espui_fan1_pwm_compID;
uint16_t espui_fan1_rpm_compID;
uint16_t espui_fan2_pwm_compID;
uint16_t espui_fan2_rpm_compID;
uint16_t espui_display_brightness_compID;
uint16_t espui_display_sleep_compID;
uint16_t espui_flash_save_compID;
uint16_t espui_temperature_warning_compID;
uint16_t espui_temperature_danger_compID;
uint16_t espui_temperature_warningThreshhold_compID;
uint16_t espui_temperature_dangerThreshhold_compID;

void fill_solid_fromTo(struct CRGB * leds, int fromToFill, int toToFill, const struct CRGB& color);
void setLed2Color(int color);
void setLed2Mode(String mode);
void setDisplaySleep(bool value);
void setDisplayBrightness(int value);
void saveToFlash();
void setSensorGraphValues(int sensorID, float temperature);
void setSensorValues(int sensorID, float temperature, float humidity);
void setFanPwm(int fanID, int pwmValue);
void setFanRpm(int fanID, int rpm);
void setTempWarnDang(int state, int warnOrDanger);
void setTempWarnDangThreshold(int state, int warnOrDanger);
void loadFromFlash();
char* string2char(String command);
void HandleTempWarn();
void HandleDisplay();
String convertToLed2ModeString(int mode);
int getFanSpeed(int fanID);
void setupWifiAndUI();

//Interupt for fan tacho
void IRAM_ATTR interupt_fan1()
{
  half_revolutionsFan1++;
}
void IRAM_ATTR interupt_fan2()
{
  half_revolutionsFan2++;
}

void wifiAP_CALLBACK (WiFiManager *myWiFiManager) {
  if(rebootOnHotspot){
      delay(200);
      ESP.restart();
  }
  else{
    myNextion.sendCommand("page message_page");
    delay(10);
    myNextion.setComponentText("message_page.tf_headline", "Hotspot was created!");
    myNextion.setComponentText("message_page.tf_message1", "Mode: Station");
    myNextion.setComponentText("message_page.tf_message2", String("SSID: " + String(wifi_ap_ssid)));
    if(wifiPassSecret)
      myNextion.setComponentText("message_page.tf_message3", String("pass: " + String("********")));
    else
      myNextion.setComponentText("message_page.tf_message3", String("pass: " + String(wifi_ap_password)));
    
    myNextion.setComponentText("message_page.tf_message4", String("IP: " + String(WiFi.softAPIP().toString())));
    currentPage = MESSAGE_PAGE;
    Serial.println("Entered WiFi config mode");
    Serial.println(WiFi.softAPIP());

    Serial.println(myWiFiManager->getConfigPortalSSID());
  }
  
}
void espui_button_wifiReset_CALLBACK(Control* sender, int value){
  wifiManager.resetSettings();
  delay(500);
  ESP.restart();
}
void espui_switch_led1_control_CALLBACK(Control* sender, int value) {
  int buttonValue = sender->value.toInt();
  if(buttonValue == 0){
    led1_relay.off();
  }
  else if(buttonValue == 1){
    led1_relay.on();
  }
  myNextion.setComponentValue("led_page.btn_led1", buttonValue);
}
void espui_button_reboot_CALLBACK(Control* sender, int value){
  if(value == B_UP){
    delay(300);
    ESP.restart();
  }
}
void espui_slider_fan1PWM_CALLBACK(Control* sender, int value) {
  int sliderValue = sender->value.toInt();
  if(sliderValue != fan1_dutyCycle){
    setFanPwm(0, sliderValue);
  }
}
void espui_slider_fan2PWM_CALLBACK(Control* sender, int value) {
  int sliderValue = sender->value.toInt();
  if(fan2_dutyCycle != sliderValue){
    setFanPwm(1, sliderValue);
  }
}
void espui_select_led2Color_CALLBACK(Control* sender, int value) {
  setLed2Color(sender->value.toInt());
}
void espui_select_led2Mode_CALLBACK(Control* sender, int value){
  setLed2Mode(sender->value);
}
void espui_slider_displayBrightness_CALLBACK(Control* sender, int value){
  display_brightness = sender->value.toInt();
  setDisplayBrightness(display_brightness);
}
void espui_switch_displaySleep_CALLBACK(Control* sender, int value){
  display_sleepState = sender->value.toInt();
  setDisplaySleep(display_sleepState);
}
void espui_button_saveToFlash_CALLBACK(Control* sender, int value){
  if(value == B_UP){
    saveToFlash();
  }
}
void espui_switch_TempWarning_CALLBACK(Control* sender, int value){
  temperature_warnState = sender->value.toInt();
  setTempWarnDang(temperature_warnState, 0);
}
void espui_switch_TempDanger_CALLBACK(Control* sender, int value){
  temperature_dangState = sender->value.toInt();
  setTempWarnDang(temperature_dangState, 1);
}
void espui_number_TempWarningThreshold_CALLBACK(Control* sender, int value){
  temperature_warnThreshold = sender->value.toInt();
  setTempWarnDangThreshold(temperature_warnThreshold, 0);
}
void espui_number_TempDangerThreshold_CALLBACK(Control* sender, int value){
  temperature_dangThreshold = sender->value.toInt();
  setTempWarnDangThreshold(temperature_dangThreshold, 1);
}

void Led2EffectsHandler( void * parameter) {
  for(;;) {
    if(led2_color_selected != CRGB::Black){
      if(led2_mode == "fade"){
        unsigned long led2_effect_currentMillis = millis();
        if (led2_effect_currentMillis - led2_effect_previousMillis >= 8) {
          for(int i = 0; i < led2_numberOfLEDs; i++ )
          {
            leds[i] = led2_color_selected;
            leds[i].fadeLightBy(triwave8(led2_effect_brightness));
          }
          FastLED.show();
          led2_effect_brightness = led2_effect_brightness + led2_effect_fadeAmount;
          // reverse the direction of the fading at the ends of the fade: 
          if(led2_effect_brightness >= 255)
          {
            led2_effect_brightness = 0; 
          }    
        }
      }
      else if(led2_mode == "running"){
        unsigned long led2_effect_currentMillis = millis();
        if (led2_effect_currentMillis - led2_effect_previousMillis >= 50) {
          led2_effect_previousMillis = led2_effect_currentMillis;
          if(led2_effect_counter1 <= led2_numberOfLEDs){
            leds[led2_effect_counter1] = led2_color_selected;
            if(led2_effect_counter1 > 0){
              leds[led2_effect_counter1 - 1] = CRGB::Black;
            }
            led2_effect_counter1++;
          }

          if(led2_effect_counter1 >= led2_numberOfLEDs){
            leds[led2_effect_counter2] = led2_color_selected;
            if(led2_effect_counter2 < led2_numberOfLEDs){
              leds[led2_effect_counter2 + 1] = CRGB::Black;
            }
            led2_effect_counter2--;
          }

          if(led2_effect_counter1 > led2_numberOfLEDs && led2_effect_counter2 < 0){
            led2_effect_counter1 = 0;
            led2_effect_counter2 = led2_numberOfLEDs;
          }
          FastLED.show();
        }
      }
      else if(led2_mode == "pulse"){
        unsigned long led2_effect_currentMillis = millis();
        if (led2_effect_currentMillis - led2_effect_previousMillis >= 50) {
          led2_effect_previousMillis = led2_effect_currentMillis;
          if(led2_effect_counter1 <= led2_numberOfLEDs){
            leds[led2_effect_counter1] = led2_color_selected;
            led2_effect_counter1++;
          }
          if(led2_effect_counter1 >= led2_numberOfLEDs){
            leds[led2_effect_counter2] = CRGB::Black;
            led2_effect_counter2--;
          }

          if(led2_effect_counter1 > led2_numberOfLEDs && led2_effect_counter2 < 0){
            led2_effect_counter1 = 0;
            led2_effect_counter2 = led2_numberOfLEDs;
          }
          FastLED.show();
        }
      }
    }
    if(led2_mode == "rainbow"){
      uint8_t fillNumber = beat8(100,300);
      fill_rainbow(leds, led2_numberOfLEDs, fillNumber, 255/led2_numberOfLEDs); 
      fadeToBlackBy(leds, led2_numberOfLEDs, led2_brightness);
      FastLED.show();
    }
    else if(led2_mode == "temperature"){
      if(dht_1_temperature <= 0){fill_solid_fromTo(leds, 0, 29, CRGB::SkyBlue);}
      else if(dht_1_temperature < temperature_warnThreshold){fill_solid_fromTo(leds, 0, 29, CRGB::Green);}
      else if(dht_1_temperature < temperature_dangThreshold){fill_solid_fromTo(leds, 0, 29, CRGB::OrangeRed);}
      else if(dht_1_temperature > temperature_dangThreshold){fill_solid_fromTo(leds, 0, 29, CRGB::Red);}

      if(dht_2_temperature <= 0){fill_solid_fromTo(leds, 30, 60, CRGB::SkyBlue);}
      else if(dht_2_temperature < temperature_warnThreshold){fill_solid_fromTo(leds, 30, 60, CRGB::Green);}
      else if(dht_2_temperature < temperature_dangThreshold){fill_solid_fromTo(leds, 30, 60, CRGB::OrangeRed);}
      else if(dht_2_temperature > temperature_dangThreshold){fill_solid_fromTo(leds, 30, 60, CRGB::Red);}
      FastLED.show();
    }
  }
}
void SensorHandler( void * parameter){
  for(;;) {
    unsigned long dht_current_millis = millis();
    if (dht_current_millis - dht_previous_millis >= dht_sense_interval) {
      dht_previous_millis = dht_current_millis;
      float tmp_humidity_1 = dht_1.readHumidity();
      float tmp_temperature_1 = dht_1.readTemperature();
      if (isnan(tmp_humidity_1) || isnan(tmp_temperature_1)){

      }
      else{
        dht_1_humidity = tmp_humidity_1;
        dht_1_temperature = tmp_temperature_1;
      }

      float tmp_humidity_2 = dht_2.readHumidity();
      float tmp_temperature_2 = dht_2.readTemperature(); 

      if (isnan(tmp_humidity_2) || isnan(tmp_temperature_2)){
        
      }
      else{
        dht_2_humidity = tmp_humidity_2;
        dht_2_temperature = tmp_temperature_2;
      }
    }
  }
}

void setup() {
  
  Serial.begin(115200);
  while(!Serial){}
  Serial2.begin(115200, SERIAL_8N1, 16, 17);
  while(!Serial2){}
  delay(300);
  
  pinMode(2, OUTPUT);
  
  pinMode(dht22_1_pin, INPUT);
  pinMode(dht22_2_pin, INPUT);

  pinMode(fan1_tacho_pin, INPUT);
  pinMode(fan1_pwm_pin, OUTPUT);
  pinMode(fan2_tacho_pin, INPUT);
  pinMode(fan2_pwm_pin, OUTPUT); 
  pinMode(buzzer_pin, OUTPUT);
  pinMode(relay_in_pin, OUTPUT);
  pinMode(led2_data_pin, OUTPUT);

  int task1return = xTaskCreatePinnedToCore(Led2EffectsHandler, "Led2EffectsTask", 10000, NULL, 1, &Led2EffectsTask, 1);
  int task2return = xTaskCreatePinnedToCore(SensorHandler, "SensorTask", 10000, NULL, 2, &SensorTask, 0);
  if(task1return) {
    Serial.println("Led2Effects Task created...");
  } else {
    Serial.printf("Couldn't create Led2Effects Task");
  }

  if(task2return) {
    Serial.println("Sensor Task created...");
  } else {
    Serial.printf("Couldn't create Sensor Task");
  }

  dht_1.begin();
  dht_2.begin();
  led1_relay.begin();

  ledcSetup(0, 25000, 8);
  ledcSetup(1, 25000, 8);
  //ledcSetup(2, 200, 8);

  ledcAttachPin(fan1_pwm_pin, 0);
  ledcAttachPin(fan2_pwm_pin, 1);
  //ledcAttachPin(buzzer_pin, 2);
  ledcWrite(0, 200);
  ledcWrite(1, 200);
  attachInterrupt(digitalPinToInterrupt(fan1_tacho_pin), interupt_fan1, FALLING);
  attachInterrupt(digitalPinToInterrupt(fan2_tacho_pin), interupt_fan2, FALLING);

  FastLED.addLeds<WS2812B, led2_data_pin, GRB>(leds, led2_numberOfLEDs);
  FastLED.setBrightness(led2_brightness);
  FastLED.setMaxPowerInVoltsAndMilliamps(5, led2_currentLimit);
  FastLED.clear(true);

  myNextion.init("boot_page");
  String esp32Version = "ESP32: v";
  esp32Version.concat(esp32_version);
  myNextion.setComponentText("about_page.tf_esp32_v", esp32Version);

  String displayVersion = "Display: v";
  displayVersion.concat(display_version);
  myNextion.setComponentText("about_page.tf_display_v", displayVersion);

  setupWifiAndUI();
  loadFromFlash();
}

void loop() {
  fan1rpm = getFanSpeed(1);
  fan2rpm = getFanSpeed(2);
  HandleTempWarn();
  HandleDisplay();
}

int getFanSpeed(int fanID){
  if(fanID == 1){
    if (half_revolutionsFan1 >= 40) { 
      fan1rpm = 30*1000/(millis() - timeoldFan1)*half_revolutionsFan1;
      timeoldFan1 = millis();
      half_revolutionsFan1 = 0; 
    }
    if(fan1_dutyCycle == 0){
      return 0;
    }
    else{
      return fan1rpm;
    }
  }
  else if(fanID == 2){
    if (half_revolutionsFan2 >= 40) { 
      fan2rpm = 30*1000/(millis() - timeoldFan2)*half_revolutionsFan2;
      timeoldFan2 = millis();
      half_revolutionsFan2 = 0; 
    }
    if(fan1_dutyCycle == 0){
      return 0;
    }
    else{
      return fan2rpm;
    }
  }
  else
    return 0;
}
void HandleTempWarn(){
  unsigned long temperature_warner_currentMillis = millis();
  if (temperature_warner_currentMillis - temperature_warner_previousMillis >= temperature_warner_checkInterval) {
    temperature_warner_previousMillis = temperature_warner_currentMillis;
    if(temperature_warnState == 1){
      if(dht_1_temperature > temperature_warnThreshold || dht_2_temperature > temperature_warnThreshold){
        Serial.println("!!!> WARNING: Temperature warning threshold has been reached :WARNING <!!!");
        normalState = false;
        if(currentPage != MESSAGE_PAGE){
          currentPage = MESSAGE_PAGE;
          
          ESPUI.updateLabel(espui_messageBox_compID, "WARNING: Temperature warning threshold has been reached");
          ESPUI.getControl(espui_messageBox_compID)->color = ControlColor::Carrot;
          ESPUI.updateControl(espui_messageBox_compID);

          myNextion.sendCommand("sleep=0");
          delay(10);
          myNextion.setComponentText("message_page.tf_headline", "Temperature Warning");
          myNextion.setComponentText("message_page.tf_message1", "Values when Triggered: ");
          myNextion.setComponentText("message_page.tf_message2", String("Warning Threshold: " + String(temperature_warnThreshold)));
          myNextion.setComponentText("message_page.tf_message3", String("Temperature1: " + String(dht_1_temperature)));
          myNextion.setComponentText("message_page.tf_message4", String("Temperature2: " + String(dht_2_temperature)));
          myNextion.sendCommand("page message_page");
          delay(10);
        }
        
      }
    }
    else if(temperature_dangState == 1){
      if(dht_1_temperature > temperature_dangThreshold || dht_2_temperature > temperature_dangThreshold){
        Serial.println("!!!> DANGER: Temperature danger threshold has been reached :DANGER <!!!");
        normalState = false;
        if(currentPage != MESSAGE_PAGE){
          currentPage = MESSAGE_PAGE;
          
          ESPUI.updateLabel(espui_messageBox_compID, "DANGER: Temperature danger threshold has been reached");
          ESPUI.getControl(espui_messageBox_compID)->color = ControlColor::Alizarin;
          ESPUI.updateControl(espui_messageBox_compID);

          myNextion.sendCommand("sleep=0");
          delay(10);
          myNextion.setComponentText("message_page.tf_headline", "Temperature Danger");
          myNextion.setComponentText("message_page.tf_message1", "Values when Triggered: ");
          myNextion.setComponentText("message_page.tf_message2", String("Danger Threshold: " + String(temperature_dangThreshold)));
          myNextion.setComponentText("message_page.tf_message3", String("Temperature1: " + String(dht_1_temperature) + String("°C")));
          myNextion.setComponentText("message_page.tf_message4", String("Temperature2: " + String(dht_2_temperature) + String("°C")));
          myNextion.sendCommand("page message_page");
          delay(10);
        }
      }
      //ledcWrite(2, 230);

    }
    else{
      if(normalState == false){
        normalState = true;
        currentPage = MAIN_PAGE;
        myNextion.sendCommand("page main_page");
        delay(10);
        ESPUI.updateLabel(espui_messageBox_compID, "");
        ESPUI.getControl(espui_messageBox_compID)->color = ControlColor::Emerald;
        ESPUI.updateControl(espui_messageBox_compID);
      }
      
    }
  }
}
void HandleDisplay(){
String message = myNextion.listen();
  if(message != ""){
    if(message == "65 1 1 0 ff ff ff"){currentPage = FANS_PAGE;}
    else if(message == "65 1 2 0 ff ff ff"){currentPage = SENSOR_PAGE;}
    else if(message == "65 1 3 0 ff ff ff"){currentPage = LED_PAGE;}
    else if(message == "65 1 4 0 ff ff ff"){currentPage = CONF_PAGE;}
    else if(message == "65 5 2 0 ff ff ff" || message == "65 6 6 0 ff ff ff"){currentPage = ABOUT_PAGE;}
    else if(message == "65 7 6 0 ff ff ff"){currentPage = MAIN_PAGE;}
    else if(message == "65 7 7 0 ff ff ff"){ESP.restart();}
    else if(message == "65 2 6 0 ff ff ff" || message == "65 3 2 0 ff ff ff" || message == "65 4 e 0 ff ff ff" || message == "65 5 1 0 ff ff ff"){currentPage = MAIN_PAGE;}
    else if(message == "65 2 4 0 ff ff ff"){setFanPwm(0, myNextion.getComponentValue("fans_page.sli_speed_fan1"));}
    else if(message == "65 2 3 0 ff ff ff"){setFanPwm(1, myNextion.getComponentValue("fans_page.sli_speed_fan2"));}
    else if(message == "65 4 b 0 ff ff ff"){led2_mode_selected++; setLed2Mode(convertToLed2ModeString(led2_mode_selected));}
    else if(message == "65 4 c 0 ff ff ff"){led2_mode_selected--; setLed2Mode(convertToLed2ModeString(led2_mode_selected));}
    else if(message == "65 4 2 0 ff ff ff"){setLed2Color(CRGB::Black);}
    else if(message == "65 4 a 0 ff ff ff"){setLed2Color(CRGB::White);}
    else if(message == "65 4 3 0 ff ff ff"){setLed2Color(CRGB::Red);}
    else if(message == "65 4 4 0 ff ff ff"){setLed2Color(CRGB::OrangeRed);}
    else if(message == "65 4 5 0 ff ff ff"){setLed2Color(CRGB::Yellow);}
    else if(message == "65 4 6 0 ff ff ff"){setLed2Color(CRGB::Green);}
    else if(message == "65 4 7 0 ff ff ff"){setLed2Color(CRGB::Blue);}
    else if(message == "65 4 8 0 ff ff ff"){setLed2Color(CRGB::Purple);}
    else if(message == "65 4 9 0 ff ff ff"){setLed2Color(CRGB::DeepPink);}
    else if(message == "65 5 4 0 ff ff ff"){display_sleepState = !display_sleepState; setDisplaySleep(display_sleepState);}
    else if(message == "65 5 3 0 ff ff ff"){display_brightness = myNextion.getComponentValue("conf_page.sli_brightness"); setDisplayBrightness(display_brightness);}
    else if(message == "65 5 e 0 ff ff ff"){saveToFlash();}
    else if(message == "65 5 6 0 ff ff ff"){temperature_warnState = !temperature_warnState; setTempWarnDang(temperature_warnState, 0);}
    else if(message == "65 5 7 0 ff ff ff"){temperature_dangState = !temperature_dangState; setTempWarnDang(temperature_dangState, 1);}
    else if(message == "65 5 a 0 ff ff ff"){temperature_warnThreshold -= 1; setTempWarnDangThreshold(temperature_warnThreshold, 0);}
    else if(message == "65 5 b 0 ff ff ff"){temperature_warnThreshold += 1; setTempWarnDangThreshold(temperature_warnThreshold, 0);}
    else if(message == "65 5 c 0 ff ff ff"){temperature_dangThreshold -= 1; setTempWarnDangThreshold(temperature_dangThreshold, 1);}
    else if(message == "65 5 d 0 ff ff ff"){temperature_dangThreshold += 1; setTempWarnDangThreshold(temperature_dangThreshold, 1);}
    else if(message == "65 4 1 0 ff ff ff"){
      int btn_led1_value = myNextion.getComponentValue("led_page.btn_led1");
      if(btn_led1_value != -1){

        if(btn_led1_value == 1){led1_relay.on();}
        else{led1_relay.off();}

        ESPUI.updateSwitcher(espui_led1Control_compID, btn_led1_value);
      }
    }
  }
  unsigned long display_current_millis = millis();
  if (display_current_millis - display_previous_millis >= display_update_interval) {
    display_previous_millis = display_current_millis;
    if(currentPage == MAIN_PAGE){
      if(fan1rpm != send_fan1rpm){
        send_fan1rpm = fan1rpm;
        setFanRpm(0, fan1rpm);
      }
      if(fan2rpm != send_fan2rpm){
        send_fan2rpm = fan2rpm;
        setFanRpm(1, fan2rpm);
      }
      if((dht_1_temperature != send_dht_1_temperature) || (dht_1_humidity != send_dht_1_humidity)){
        send_dht_1_temperature = dht_1_temperature;
        send_dht_1_humidity = dht_1_humidity;
        setSensorValues(0, dht_1_temperature, dht_1_humidity);
      }
      if((dht_2_temperature != send_dht_2_temperature) || (dht_2_humidity != send_dht_2_humidity)){
        send_dht_2_temperature = dht_2_temperature;
        send_dht_2_humidity = dht_2_humidity;
        setSensorValues(1, dht_2_temperature, dht_2_humidity);
      }
    }
    else if(currentPage == FANS_PAGE){
      if(fan1rpm != send_fan1rpm){
        send_fan1rpm = fan1rpm;
        setFanRpm(0, fan2rpm);
      }
      if(fan2rpm != send_fan2rpm){
        send_fan2rpm = fan2rpm;
        setFanRpm(1, fan2rpm);
      }
    }
    else if(currentPage == SENSOR_PAGE){
      if((dht_1_temperature != send_dht_1_temperature) || (dht_1_humidity != send_dht_1_humidity)){
        send_dht_1_temperature = dht_1_temperature;
        send_dht_1_humidity = dht_1_humidity;
        setSensorValues(0, dht_1_temperature, dht_1_humidity);
      }
      if((dht_2_temperature != send_dht_2_temperature) || (dht_2_humidity != send_dht_2_humidity)){
        send_dht_2_temperature = dht_2_temperature;
        send_dht_2_humidity = dht_2_humidity;
        setSensorValues(0, dht_2_temperature, dht_2_humidity);
      }
    }
    if(dht_1_temperature >= dht_1_minTemp){
      setSensorGraphValues(0, dht_1_temperature);
    }
    if(dht_2_temperature >= dht_2_minTemp){
      setSensorGraphValues(1, dht_2_temperature);
    }
  }
}
String convertToLed2ModeString(int mode){
  String returnValue = "solid";
  if(mode >= led2_numberOfModes -1){
    mode = led2_numberOfModes -1;
  }
  else if(mode <= 0){
    mode = 0;
  }

  switch(mode){
    case 0: returnValue = "solid"; break;
    case 1: returnValue = "fade"; break;
    case 2: returnValue = "rainbow"; break;
    case 3: returnValue = "running"; break;
    case 4: returnValue = "pulse"; break;
    case 5: returnValue = "temperature"; break;
  }
  return returnValue;
}
char* string2char(String command){
  char *p = const_cast<char*>(command.c_str());
  return p;
}
void setupWifiAndUI(){
  wifiManager.setAPCallback(wifiAP_CALLBACK);
  wifiManager.setAPStaticIPConfig(IPAddress(192, 168, 200, 1), IPAddress(192, 168, 200, 1), IPAddress(255,255,255,0));
  wifiManager.setConfigPortalBlocking(wifiManagerBlocking);
  if(wifiManagerBlocking)
    wifiManager.setConfigPortalTimeout(wifiManagerTimeout);
  wifiManager.setDebugOutput(wifiManagerDebug);
  wifiManager.setHostname(wifi_hostname);
  if(wifiManagerDarkMode)
    wifiManager.setClass("invert");
  


  wifiManager.autoConnect(wifi_ap_ssid, wifi_ap_password);
  currentPage = MAIN_PAGE;
  myNextion.sendCommand("page main_page");
  delay(10);

  Serial.println("Setting up WiFi done!");
  //Tabs
  uint16_t mainTab = ESPUI.addControl(ControlType::Tab, "Main Page", "Main Page");
  uint16_t confTab = ESPUI.addControl(ControlType::Tab, "Configuration", "Configuration");
  uint16_t aboutTab = ESPUI.addControl(ControlType::Tab, "About", "About");
  
  //Temperature graphs
  //espui_sens1Graph_compID = ESPUI.addControl(ControlType::Graph, "Sensor 1 Temperature Graph", "", ControlColor::Peterriver, mainTab);
  //espui_sens2Graph_compID = ESPUI.addControl(ControlType::Graph, "Sensor 2 Temperature Graph", "", ControlColor::Peterriver, mainTab);
  //ESPUI.clearGraph(espui_sens1Graph_compID);
  //ESPUI.clearGraph(espui_sens2Graph_compID);

  espui_messageBox_compID = ESPUI.label("Status/Messages", ControlColor::Peterriver, ""); 
  ESPUI.button("Reboot ESP32", &espui_button_reboot_CALLBACK, ControlColor::Peterriver, "Reboot");
  ESPUI.button("Reset WiFi config", &espui_button_wifiReset_CALLBACK, ControlColor::Peterriver, "Reset config");
  
  //LED1 Control
  espui_led1Control_compID = ESPUI.addControl(ControlType::Switcher, "LED 1 Control", "", ControlColor::Peterriver, mainTab, &espui_switch_led1_control_CALLBACK);
  
  //LED2 color selector
  espui_led2ColorSelect_compID = ESPUI.addControl(ControlType::Select, "LED2 Color:", "", ControlColor::Peterriver, mainTab, &espui_select_led2Color_CALLBACK);
  ESPUI.addControl(ControlType::Option, "Black (OFF)", String(CRGB::Black), ControlColor::Peterriver, espui_led2ColorSelect_compID);
  ESPUI.addControl(ControlType::Option, "White", String(CRGB::White), ControlColor::Peterriver, espui_led2ColorSelect_compID);
  ESPUI.addControl(ControlType::Option, "Red", String(CRGB::Red), ControlColor::Peterriver, espui_led2ColorSelect_compID);
  ESPUI.addControl(ControlType::Option, "Orange", String(CRGB::OrangeRed), ControlColor::Peterriver, espui_led2ColorSelect_compID);
  ESPUI.addControl(ControlType::Option, "Yellow", String(CRGB::Yellow), ControlColor::Peterriver, espui_led2ColorSelect_compID);
  ESPUI.addControl(ControlType::Option, "Green", String(CRGB::Green), ControlColor::Peterriver, espui_led2ColorSelect_compID);
  ESPUI.addControl(ControlType::Option, "Blue", String(CRGB::Blue), ControlColor::Peterriver, espui_led2ColorSelect_compID);
  ESPUI.addControl(ControlType::Option, "Purple", String(CRGB::Purple), ControlColor::Peterriver, espui_led2ColorSelect_compID);
  ESPUI.addControl(ControlType::Option, "Pink", String(CRGB::DeepPink), ControlColor::Peterriver, espui_led2ColorSelect_compID);

  //LED2 mode selector
  espui_led2ModeSelect_compID = ESPUI.addControl(ControlType::Select, "LED2 Mode:", "", ControlColor::Peterriver, mainTab, &espui_select_led2Mode_CALLBACK);
  ESPUI.addControl(ControlType::Option, "SOLID -> needs LED2 Color", "solid", ControlColor::Peterriver, espui_led2ModeSelect_compID);
  ESPUI.addControl(ControlType::Option, "FADE -> needs LED2 Color", "fade", ControlColor::Peterriver, espui_led2ModeSelect_compID);
  ESPUI.addControl(ControlType::Option, "RUNNING -> needs LED2 Color", "running", ControlColor::Peterriver, espui_led2ModeSelect_compID);
  ESPUI.addControl(ControlType::Option, "PULSE -> needs LED2 Color", "pulse", ControlColor::Peterriver, espui_led2ModeSelect_compID);
  ESPUI.addControl(ControlType::Option, "TEMPERATURE", "temperature", ControlColor::Peterriver, espui_led2ModeSelect_compID);
  ESPUI.addControl(ControlType::Option, "RAINBOW", "rainbow", ControlColor::Peterriver, espui_led2ModeSelect_compID);
  //Sensor value display
  espui_sensor1_temp_compID = ESPUI.addControl(ControlType::Label, "Sensor 1 Temperature", "0°C", ControlColor::Peterriver, mainTab);
  espui_sensor1_humi_compID = ESPUI.addControl(ControlType::Label, "Sensor 1 Humidity", "0%", ControlColor::Peterriver, mainTab);
  
  espui_sensor2_temp_compID = ESPUI.addControl(ControlType::Label, "Sensor 2 Temperature", "0°C", ControlColor::Peterriver, mainTab);
  espui_sensor2_humi_compID = ESPUI.addControl(ControlType::Label, "Sensor 2 Humidity", "0%", ControlColor::Peterriver, mainTab);

  //Fan Control
  espui_fan1_pwm_compID = ESPUI.addControl(ControlType::Slider, "Fan 1 PWM", "", ControlColor::Peterriver, mainTab, &espui_slider_fan1PWM_CALLBACK);
  espui_fan1_rpm_compID = ESPUI.addControl(ControlType::Label, "Fan 1 RPM", "0rpm", ControlColor::Peterriver, mainTab);
  espui_fan2_pwm_compID = ESPUI.addControl(ControlType::Slider, "Fan 2 PWM", "", ControlColor::Peterriver, mainTab, &espui_slider_fan2PWM_CALLBACK);
  espui_fan2_rpm_compID = ESPUI.addControl(ControlType::Label, "Fan 2 RPM", "0rpm", ControlColor::Peterriver, mainTab);

  //Configuration Tab
  espui_temperature_warning_compID = ESPUI.addControl(ControlType::Switcher, "Temperature Warning", "", ControlColor::Peterriver, confTab, &espui_switch_TempWarning_CALLBACK);
  espui_temperature_danger_compID = ESPUI.addControl(ControlType::Switcher, "Temperature Danger Warning", "", ControlColor::Peterriver, confTab, &espui_switch_TempDanger_CALLBACK);
  espui_temperature_warningThreshhold_compID = ESPUI.addControl(ControlType::Number, "Temperature Warning Threshold", String(temperature_warnThreshold), ControlColor::Peterriver, confTab, &espui_number_TempWarningThreshold_CALLBACK);
  espui_temperature_dangerThreshhold_compID = ESPUI.addControl(ControlType::Number, "Temperature Danger Threshold", String(temperature_dangThreshold), ControlColor::Peterriver, confTab, &espui_number_TempDangerThreshold_CALLBACK);

  espui_display_brightness_compID = ESPUI.addControl(ControlType::Slider, "Display Brightness", "", ControlColor::Peterriver, confTab, &espui_slider_displayBrightness_CALLBACK);
  espui_display_sleep_compID = ESPUI.addControl(ControlType::Switcher, "Display Sleep", "", ControlColor::Peterriver, confTab, &espui_switch_displaySleep_CALLBACK);
  espui_flash_save_compID = ESPUI.addControl(ControlType::Button, "Save Settings to Flash", "Save to Flash", ControlColor::Peterriver, confTab, &espui_button_saveToFlash_CALLBACK);

  //About Tab
  ESPUI.addControl(ControlType::Label, "Made by:", "Marvin Beym", ControlColor::Peterriver, aboutTab);
  ESPUI.addControl(ControlType::Label, "GitHub:", "https://github.com/MarvinBeym/3D-Printer_Enclosure-Controller_ESP32", ControlColor::Peterriver, aboutTab);
  ESPUI.addControl(ControlType::Label, "ESP32 Software version:", esp32_version, ControlColor::Peterriver, aboutTab);
  ESPUI.addControl(ControlType::Label, "Display Software version:", display_version, ControlColor::Peterriver, aboutTab);

  ESPUI.begin("Enclosure Controller ESP32");
  Serial.println("Setting up Webinterface done!");
}
void setFanPwm(int fanID, int pwmValue){
  if(fanID == 0){
    fan1_dutyCycle = pwmValue;
    myNextion.setComponentText("main_page.tf_pwm_fan1", String(fan1_dutyCycle) + "%");
    myNextion.setComponentText("fans_page.tf_pwm_fan1", String(fan1_dutyCycle) + "%");
    myNextion.setComponentValue("fans_page.sli_speed_fan1", fan1_dutyCycle);
    ESPUI.updateSlider(espui_fan1_pwm_compID, fan1_dutyCycle);
    int convertedValue = map(fan1_dutyCycle, 0, 100, 0, 255);
    ledcWrite(0, convertedValue);
  }
  else if(fanID == 1){
    fan2_dutyCycle = pwmValue;
    myNextion.setComponentText("main_page.tf_pwm_fan2", String(fan2_dutyCycle) + "%");
    myNextion.setComponentText("fans_page.tf_pwm_fan2", String(fan2_dutyCycle) + "%");
    myNextion.setComponentValue("fans_page.sli_speed_fan2", fan2_dutyCycle);
    ESPUI.updateSlider(espui_fan2_pwm_compID, fan2_dutyCycle);
    int convertedValue = map(fan2_dutyCycle, 0, 100, 0, 255);
    ledcWrite(1, convertedValue);
  }
  
}
void setFanRpm(int fanID, int rpm){
  if(fanID == 0){
    ESPUI.updateLabel(espui_fan1_rpm_compID, String(rpm) + "rpm");
    myNextion.setComponentText("main_page.tf_speed_fan1", String(rpm) + "rpm");
    myNextion.setComponentText("fans_page.tf_speed_fan1", String(rpm) + "rpm");
  }
  else if(fanID == 1){
    ESPUI.updateLabel(espui_fan2_rpm_compID, String(rpm) + "rpm");
    myNextion.setComponentText("main_page.tf_speed_fan2", String(rpm) + "rpm");
    myNextion.setComponentText("fans_page.tf_speed_fan2", String(rpm) + "rpm");
  }
}
void setSensorValues(int sensorID, float temperature, float humidity){
  if(sensorID == 0){
    ESPUI.updateLabel(espui_sensor1_temp_compID, String(temperature) + "°C");
    ESPUI.updateLabel(espui_sensor1_humi_compID, String(humidity) + "%");
    myNextion.setComponentText("main_page.tf_temp_sens1", String(temperature) + "°C");
    myNextion.setComponentText("main_page.tf_hum_sens1", String(humidity) + "%");
    myNextion.setComponentText("sensor_page.tf_temp_sens1", String(temperature) + "°C");
    myNextion.setComponentText("sensor_page.tf_hum_sens1", String(humidity) + "%");
  }
  else if(sensorID == 1){
    ESPUI.updateLabel(espui_sensor2_temp_compID, String(temperature) + "°C");
    ESPUI.updateLabel(espui_sensor2_humi_compID, String(humidity) + "%");
    myNextion.setComponentText("main_page.tf_temp_sens2", String(temperature) + "°C");
    myNextion.setComponentText("main_page.tf_hum_sens2", String(humidity) + "%");
    myNextion.setComponentText("sensor_page.tf_temp_sens2", String(temperature) + "°C");
    myNextion.setComponentText("sensor_page.tf_hum_sens2", String(humidity) + "%");
  }
}
void setSensorGraphValues(int sensorID, float temperature){
  if(sensorID == 0){
    //ESPUI.addGraphPoint(espui_sens1Graph_compID, 20);
    if(currentPage == SENSOR_PAGE){
      String graphCommand = "add 1,0,";
      graphCommand.concat(map(temperature, dht_1_minTemp + 128, 255, 128, 255));
      myNextion.sendCommand(string2char(graphCommand));
      delay(10);
    }
  }
  else if(sensorID == 1){
    //ESPUI.addGraphPoint(espui_sens2Graph_compID, 30);
    if(currentPage == SENSOR_PAGE){
      String graphCommand = "add 1,1,";
      graphCommand.concat(map(temperature, dht_2_minTemp + 0, 255, 0, 127));
      myNextion.sendCommand(string2char(graphCommand));
      delay(10);
    }

  }
}
void setDisplaySleep(bool value){
  if(value == false){
      myNextion.sendCommand("sleep=0");
      delay(10);
      myNextion.sendCommand("thsp=0");
      delay(10);
  }
  else{
    myNextion.sendCommand("thsp=10");
    delay(10);
  }
  myNextion.setComponentValue("conf_page.btn_sleep", value);
  ESPUI.updateSwitcher(espui_display_sleep_compID, value);
}
void setDisplayBrightness(int value){
  ESPUI.updateSlider(espui_display_brightness_compID, value);
  myNextion.sendCommand(string2char("dims=" + String(display_brightness)));
  delay(10);
}
void saveToFlash(){
  preferences.begin("enclC_E32", false);

  preferences.putInt("fan1_dC", fan1_dutyCycle);
  preferences.putInt("fan2_dC", fan2_dutyCycle);
  preferences.putInt("led1_rlyState", led1_relay.getState());
  preferences.putInt("led2_color_sel", led2_color_selected);
  preferences.putInt("led2_mode_sel", led2_mode_selected);
  preferences.putString("led2_mod", led2_mode);
  preferences.putInt("disp_brt", display_brightness);
  preferences.putInt("disp_slpState", display_sleepState);
  preferences.putBool("temp_warnState", temperature_warnState);
  preferences.putBool("temp_dangState", temperature_dangState);
  preferences.putInt("temp_warnThresh", temperature_warnThreshold);
  preferences.putInt("temp_dangThresh", temperature_dangThreshold);

  preferences.end();
  Serial.println("Finished saving to Flash!");
}
void setLed2Color(int color){
  led2_color_selected = color;
  ESPUI.updateSelect(espui_led2ColorSelect_compID, String(color));

/*
    myNextion.setComponentValue("led_page.btn_black", 0);
    delay(100);
    myNextion.setComponentValue("led_page.btn_white", 0);
    delay(100);
    myNextion.setComponentValue("led_page.btn_red", 0);
    delay(100);
    myNextion.setComponentValue("led_page.btn_orange", 0);
    delay(100);
    myNextion.setComponentValue("led_page.btn_yellow", 0);
    delay(100);
    myNextion.setComponentValue("led_page.btn_green", 0);
    delay(100);
    myNextion.setComponentValue("led_page.btn_blue", 0);
    delay(100);
    myNextion.setComponentValue("led_page.btn_purple", 0);
    delay(100);
    myNextion.setComponentValue("led_page.btn_pink", 0);
    delay(100);
*/
    switch (color){
    case CRGB::Black:
      myNextion.setComponentValue("led_page.btn_black", 1);
      break;
    case CRGB::White:
      myNextion.setComponentValue("led_page.btn_white", 1);
      break;
    case CRGB::Red:
      myNextion.setComponentValue("led_page.btn_red", 1);
      break;
    case CRGB::OrangeRed:
      myNextion.setComponentValue("led_page.btn_orange", 1);
      break;
    case CRGB::Yellow:
      myNextion.setComponentValue("led_page.btn_yellow", 1);
      break;
    case CRGB::Green:
      myNextion.setComponentValue("led_page.btn_green", 1);
      break;
    case CRGB::Blue:
      myNextion.setComponentValue("led_page.btn_blue", 1);
      break;
    case CRGB::Purple:
      myNextion.setComponentValue("led_page.btn_purple", 1);
      break;
    case CRGB::DeepPink:
      myNextion.setComponentValue("led_page.btn_pink", 1);
      break;
    }
    for(int i = 0; i < led2_numberOfLEDs; i++){
      leds[i] = led2_color_selected;
    }
    FastLED.setBrightness(led2_brightness);
    FastLED.show();
  
}
void setLed2Mode(String mode){
  ESPUI.updateSelect(espui_led2ModeSelect_compID, mode);
  led2_mode = mode;
  led2_effect_counter1 = 0;
  led2_effect_counter2 = led2_numberOfLEDs;
  FastLED.clear(true);
  mode.toUpperCase();
  myNextion.setComponentText("led_page.tf_mode", mode);
}
void setTempWarnDang(int state, int warnOrDanger){
  switch(warnOrDanger){
    case 0:
      if(temperature_dangState == 1 && state == 1){
        temperature_dangState = 0;
        ESPUI.updateSwitcher(espui_temperature_danger_compID, 0); 
        myNextion.setComponentValue("conf_page.btn_tempDang", 0);
      }
      ESPUI.updateSwitcher(espui_temperature_warning_compID, state); 
      myNextion.setComponentValue("conf_page.btn_tempWarn", state);
      break;
    case 1:
      if(temperature_warnState == 1 && state == 1){
        temperature_warnState = 0;
        ESPUI.updateSwitcher(espui_temperature_warning_compID, 0); 
        myNextion.setComponentValue("conf_page.btn_tempWarn", 0);
      }
      ESPUI.updateSwitcher(espui_temperature_danger_compID, state); 
      myNextion.setComponentValue("conf_page.btn_tempDang", state);
      break;
  }
}
void setTempWarnDangThreshold(int value, int warnOrDanger){ 
  switch(warnOrDanger){
    case 0:
      if(value >= temperature_dangThreshold){value = temperature_dangThreshold - 1; temperature_warnThreshold = value;}
      ESPUI.updateNumber(espui_temperature_warningThreshhold_compID, value); 
      myNextion.setComponentValue("conf_page.nbr_tempWT", value);
      break;
    case 1:
      if(value <= temperature_warnThreshold){value = temperature_warnThreshold + 1; temperature_dangThreshold = value;}
      ESPUI.updateNumber(espui_temperature_dangerThreshhold_compID, value); 
      myNextion.setComponentValue("conf_page.nbr_tempDT", value);
      break;
  }
}
void fill_solid_fromTo(struct CRGB * leds, int fromToFill, int toToFill, const struct CRGB& color){
  for( int i = fromToFill; i < toToFill; i++) {
    leds[i] = color;
  }
}
void loadFromFlash(){
  preferences.begin("enclC_E32", false);
  fan1_dutyCycle = preferences.getInt("fan1_dC", 0);
  fan2_dutyCycle = preferences.getInt("fan2_dC", 0);
  int led1_relayState = preferences.getInt("led1_rlyState", 0);
  led2_color_selected = preferences.getInt("led2_color_sel", 0);
  led2_mode_selected = preferences.getInt("led2_mode_sel", 0);
  led2_mode = preferences.getString("led2_mod", "solid");
  display_brightness = preferences.getInt("disp_brt", 80);
  display_sleepState = preferences.getInt("disp_slpState", 0);
  temperature_warnState = preferences.getBool("temp_warnState", false);
  temperature_dangState = preferences.getBool("temp_dangState", false);
  temperature_warnThreshold = preferences.getInt("temp_warnThresh", 40);
  temperature_dangThreshold = preferences.getInt("temp_dangThresh", 60);
  preferences.end();

  setFanPwm(0, fan1_dutyCycle);
  int convertedValue = map(fan1_dutyCycle, 0, 100, 0, 255);
  ledcWrite(0, convertedValue);

  setFanPwm(1, fan2_dutyCycle);
  convertedValue = map(fan2_dutyCycle, 0, 100, 0, 255);
  ledcWrite(1, convertedValue);
  
  switch(led1_relayState){
    case 0: led1_relay.off(); break;
    case 1: led1_relay.on(); break;
  }

  setLed2Color(led2_color_selected);
  setLed2Mode(led2_mode);

  if(display_brightness <= 20 || display_brightness > 100){
    display_brightness = 80;
  }
  setDisplayBrightness(display_brightness);
  
  setDisplaySleep(display_sleepState);

  setTempWarnDang(temperature_warnState, 0);
  setTempWarnDang(temperature_dangState, 1);
  setTempWarnDangThreshold(temperature_warnThreshold, 0);
  setTempWarnDangThreshold(temperature_dangThreshold, 1);

  Serial.println("Finished loading save from Flash!");
}