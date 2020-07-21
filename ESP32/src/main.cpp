#include <Arduino.h>
#include <Preferences.h>
#include <Adafruit_Sensor.h>
#include <DHT.h>
#include <Wire.h>
#include <SPI.h>
#include <WiFi.h>
#include <DNSServer.h>
#include <FastLED.h>
#include <WiFi.h>
#include <DNSServer.h>
#include <ESPUI.h>

#include "Relay.h"
#include "PinDefinition.h"
#include "Configuration.h"

#include <Nextion.h>
#define nextion Serial2

const String version = "1.0";

//Storage
Preferences preferences;

//Wifi
const char* wifi_ssid = "***REMOVED***";
const char* wifi_password =  "***REMOVED***";

const char* wifi_ap_ssid = "enclosure_controller_esp32";
const char* wifi_ap_password = "enclosure_controller_esp32";

const char* wifi_hostname = "enclosure_controller_esp32";

uint8_t wifi_timeout = 5;

const byte DNS_PORT = 53;
IPAddress apIP(192, 168, 1, 1);
DNSServer dnsServer;

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

//LED
CRGB leds[led2_numberOfLEDs];
String led2_selectedColors[9];
int led2_mode_selected = 0;
int led2_color_selected = -1;


//Display
Nextion myNextion(nextion, 115200);

unsigned long display_previous_millis = 0;
int currentPage = 1;
int sliderValue = 0;
bool display_sleepState = 0;
int display_brightness = 80;

//ESPUI
uint16_t espui_sens1Graph_compID;
uint16_t espui_sens2Graph_compID;
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

void setLed2Color(String color);
void setLed2Mode(String mode);
void setDisplaySleep(bool value);
void setDisplayBrightness(int value);
void setSensorGraphValues(int sensorID, float temperature);
void setSensorValues(int sensorID, float temperature, float humidity);
void setFanPwm(int fanID, int pwmValue);
void setFanRpm(int fanID, int rpm);
void loadFlashSave();
char* string2char(String command);
void HandleSensors();
void HandleDisplay();
int getFanSpeed(int fanID);
void setupWifiAndUI();
void interupt_fan1()
{
  half_revolutionsFan1++;
}
void interupt_fan2()
{
  half_revolutionsFan2++;
}
void espui_button_led1_control_CALLBACK(Control* sender, int value) {
  int buttonValue = sender->value.toInt();
  if(buttonValue == 0){
    led1_relay.off();
  }
  else if(buttonValue == 1){
    led1_relay.on();
  }
  myNextion.setComponentValue("led_page.btn_led1", buttonValue);
}
void espui_slider_fan1PWM_CALLBACK(Control* sender, int value) {
  int sliderValue = sender->value.toInt();
  if(fan1_dutyCycle != sliderValue){
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
  setLed2Color(sender->value);
}
void espui_select_led2Mode_CALLBACK(Control* sender, int value){
  setLed2Mode(sender->value);
}
void espui_slider_displayBrightness_CALLBACK(Control* sender, int value){
  display_brightness = sender->value.toInt();
  setDisplayBrightness(display_brightness);
}
void espui_button_displaySleep_CALLBACK(Control* sender, int value){
  display_sleepState = !display_sleepState;
  setDisplaySleep(display_sleepState);
}
void espui_button_saveToFlash_CALLBACK(Control* sender, int value){
  preferences.begin("enclC_E32", false);

  preferences.putInt("fan1_dC", fan1_dutyCycle);
  preferences.putInt("fan2_dC", fan2_dutyCycle);
  preferences.putInt("led1_rlyState", led1_relay.getState());
  preferences.putInt("led2_color_sel", led2_color_selected);
  preferences.putInt("led2_mode_sel", led2_mode_selected);
  preferences.putInt("disp_brt", display_brightness);
  preferences.putInt("disp_slpState", display_sleepState);
  preferences.end();
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

  dht_1.begin();
  dht_2.begin();
  led1_relay.begin();

  ledcSetup(0, 25000, 8);
  ledcSetup(1, 25000, 8);

  ledcAttachPin(fan1_pwm_pin, 0);
  ledcAttachPin(fan2_pwm_pin, 1);
  ledcWrite(0, 0);
  ledcWrite(1, 0);
  attachInterrupt(digitalPinToInterrupt(fan1_tacho_pin), interupt_fan1, FALLING);
  attachInterrupt(digitalPinToInterrupt(fan2_tacho_pin), interupt_fan2, FALLING);

  FastLED.addLeds<WS2812B, led2_data_pin, GRB>(leds, led2_numberOfLEDs);
  FastLED.setBrightness(100);
  FastLED.setMaxPowerInVoltsAndMilliamps(5, led2_currentLimit);
  FastLED.clear(true);

  myNextion.init("boot_page");

  setupWifiAndUI();

  loadFlashSave();
}

void loop() {
  //dnsServer.processNextRequest();
  fan1rpm = getFanSpeed(1);
  fan2rpm = getFanSpeed(2);
  HandleSensors();
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
    if(fan1rpm <= fan1_max_rpm){
      return fan1rpm;
    }
    else{
      return fan1_max_rpm;
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
    if(fan2rpm <= fan2_max_rpm){
      return fan2rpm;
    }
    else{
      return fan2_max_rpm;
    }
  }
  else
    return 0;
}
void HandleSensors(){
  unsigned long dht_current_millis = millis();
  if (dht_current_millis - dht_previous_millis >= dht_sense_interval) {
    dht_previous_millis = dht_current_millis;

    float tmp_humidity_1 = dht_1.readHumidity();
    float tmp_temperature_1 = dht_1.readTemperature(); 
    if (isnan(tmp_humidity_1) || isnan(tmp_temperature_1)){
      return;
    }
    else{
      dht_1_humidity = tmp_humidity_1;
      dht_1_temperature = tmp_temperature_1;
    }

    float tmp_humidity_2 = dht_2.readHumidity();
    float tmp_temperature_2 = dht_2.readTemperature(); 

    if (isnan(tmp_humidity_2) || isnan(tmp_temperature_2)){
      return;
    }
    else{
      dht_2_humidity = tmp_humidity_2;
      dht_2_temperature = tmp_temperature_2;
    }
  }
}
void HandleDisplay(){
String message = myNextion.listen();
  if(message != ""){
    Serial.println(message);
    if(message == "65 1 1 0 ff ff ff"){
      currentPage = 2;
    }
    if(message == "65 1 2 0 ff ff ff"){
      currentPage = 3;
    }
    if(message == "65 1 3 0 ff ff ff"){
      currentPage = 4;
    }
    if(message == "65 1 4 0 ff ff ff"){
      currentPage = 5;
    }
    if(message == "65 5 2 0 ff ff ff"){
      currentPage = 6;
    }
    if(message == "65 2 6 0 ff ff ff" || message == "65 3 2 0 ff ff ff" || message == "65 4 e 0 ff ff ff" || message == "65 5 1 0 ff ff ff"){
      currentPage = 1;
    }
    if(message == "65 4 1 0 ff ff ff"){
      int btn_led1_value = myNextion.getComponentValue("led_page.btn_led1");
      if(btn_led1_value != -1){
        if(btn_led1_value == 1){
          led1_relay.on();
        }
        else{
          led1_relay.off();
        }
        ESPUI.updateSwitcher(espui_led1Control_compID, btn_led1_value);
      }
    }
    if(message == "65 2 4 0 ff ff ff"){
      int fan_sliderValue = myNextion.getComponentValue("fans_page.sli_speed_fan1");
      setFanPwm(0, fan_sliderValue);
    }
    if(message == "65 2 3 0 ff ff ff"){
      int fan_sliderValue = myNextion.getComponentValue("fans_page.sli_speed_fan2");
      setFanPwm(1, fan_sliderValue);
    }

    if(message == "65 4 b 0 ff ff ff"){
      String led2_mode = "solid";
      led2_mode_selected++;
      if(led2_mode_selected >= 1){
        led2_mode_selected = 1;
      }
      if(led2_mode_selected == 0){
        led2_mode = "solid";
      }
      else if(led2_mode_selected == 1){
        led2_mode = "fade";
      }
      setLed2Mode(led2_mode);
    }
    if(message == "65 4 c 0 ff ff ff"){
      String led2_mode = "solid";
      if(led2_mode_selected <= 0){
        led2_mode_selected = 0;
      }
      if(led2_mode_selected == 0){
        led2_mode = "solid"; 
      }
      else if(led2_mode_selected == 1){
        led2_mode = "fade";
      }
      setLed2Mode(led2_mode);
    }
    if(message == "65 4 a 0 ff ff ff"){setLed2Color("white");}
    if(message == "65 4 2 0 ff ff ff"){setLed2Color("black");}
    if(message == "65 4 3 0 ff ff ff"){setLed2Color("red");}
    if(message == "65 4 4 0 ff ff ff"){setLed2Color("orange");}
    if(message == "65 4 5 0 ff ff ff"){setLed2Color("yellow");}
    if(message == "65 4 6 0 ff ff ff"){setLed2Color("green");}
    if(message == "65 4 7 0 ff ff ff"){setLed2Color("blue");}
    if(message == "65 4 8 0 ff ff ff"){setLed2Color("purple");}
    if(message == "65 4 9 0 ff ff ff"){setLed2Color("pink");}

    if(message == "65 5 4 0 ff ff ff"){
      display_sleepState = !display_sleepState;
      setDisplaySleep(display_sleepState);
    }
    if(message == "65 5 3 0 ff ff ff"){
      display_brightness = myNextion.getComponentValue("conf_page.sli_brightness");
      setDisplayBrightness(display_brightness);
    }
  }
  unsigned long display_current_millis = millis();
  if (display_current_millis - display_previous_millis >= display_update_interval) {
    display_previous_millis = display_current_millis;
    if(currentPage == 1){
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
    else if(currentPage == 2){
      if(fan1rpm != send_fan1rpm){
        send_fan1rpm = fan1rpm;
        setFanRpm(0, fan2rpm);
      }
      if(fan2rpm != send_fan2rpm){
        send_fan2rpm = fan2rpm;
        setFanRpm(1, fan2rpm);
      }
    }
    else if(currentPage == 3){
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
    if(dht_1_temperature >= dht_1_min_tempGraph){
      setSensorGraphValues(0, dht_1_temperature);
    }
    if(dht_2_temperature >= dht_2_min_tempGraph){
      setSensorGraphValues(1, dht_2_temperature);
    }
  }
}
char* string2char(String command){
  char *p = const_cast<char*>(command.c_str());
  return p;
}
void setupWifiAndUI(){
  ESPUI.setVerbosity(Verbosity::Quiet);
  WiFi.setHostname(wifi_hostname);
  // try to connect to existing network
  WiFi.begin(wifi_ssid, wifi_password);
  Serial.print("\n\nTry to connect to existing network");
  {
    wifi_timeout = 5;

    do {
      delay(500);
      Serial.print(".");
      wifi_timeout--;
    } while (wifi_timeout && WiFi.status() != WL_CONNECTED);

    // not connected -> create hotspot
    if (WiFi.status() != WL_CONNECTED) {
      Serial.print("\n\nCreating hotspot");

      WiFi.mode(WIFI_AP);
      delay(2000);
      WiFi.softAPConfig(apIP, apIP, IPAddress(255, 255, 255, 0));
      WiFi.softAP(wifi_ap_ssid, wifi_ap_password);

      wifi_timeout = 5;

      do {
        delay(500);
        Serial.print(".");
        wifi_timeout--;
      } while (wifi_timeout);
    }
  }

  //dnsServer.start( DNS_PORT, "*", apIP );

  Serial.println("\n\nWiFi parameters:");
  Serial.print("Mode: ");
  Serial.println(WiFi.getMode() == WIFI_AP ? "Station" : "Client");
  Serial.print("IP address: ");
  Serial.println(WiFi.getMode() == WIFI_AP ? WiFi.softAPIP() : WiFi.localIP());

  //Tabs
  uint16_t mainTab = ESPUI.addControl(ControlType::Tab, "Main Page", "Main Page");
  uint16_t confTab = ESPUI.addControl(ControlType::Tab, "Configuration", "Configuration");
  uint16_t aboutTab = ESPUI.addControl(ControlType::Tab, "About", "About");
  
  //Temperature graphs
  espui_sens1Graph_compID = ESPUI.addControl(ControlType::Graph, "Sensor 1 Temperature Graph", "", ControlColor::Peterriver, mainTab);
  espui_sens2Graph_compID = ESPUI.addControl(ControlType::Graph, "Sensor 2 Temperature Graph", "", ControlColor::Peterriver, mainTab);
  ESPUI.clearGraph(espui_sens1Graph_compID);
  ESPUI.clearGraph(espui_sens2Graph_compID);

  //LED1 Control
  espui_led1Control_compID = ESPUI.addControl(ControlType::Switcher, "LED 1 Control", "", ControlColor::Peterriver, mainTab, &espui_button_led1_control_CALLBACK);
  
  //LED2 color selector
  espui_led2ColorSelect_compID = ESPUI.addControl(ControlType::Select, "LED2 Color:", "", ControlColor::Peterriver, mainTab, &espui_select_led2Color_CALLBACK);
  ESPUI.addControl(ControlType::Option, "Black (OFF)", "black", ControlColor::Peterriver, espui_led2ColorSelect_compID);
  ESPUI.addControl(ControlType::Option, "White", "white", ControlColor::Peterriver, espui_led2ColorSelect_compID);
  ESPUI.addControl(ControlType::Option, "Red", "red", ControlColor::Peterriver, espui_led2ColorSelect_compID);
  ESPUI.addControl(ControlType::Option, "Orange", "orange", ControlColor::Peterriver, espui_led2ColorSelect_compID);
  ESPUI.addControl(ControlType::Option, "Yellow", "yellow", ControlColor::Peterriver, espui_led2ColorSelect_compID);
  ESPUI.addControl(ControlType::Option, "Green", "green", ControlColor::Peterriver, espui_led2ColorSelect_compID);
  ESPUI.addControl(ControlType::Option, "Blue", "blue", ControlColor::Peterriver, espui_led2ColorSelect_compID);
  ESPUI.addControl(ControlType::Option, "Purple", "purple", ControlColor::Peterriver, espui_led2ColorSelect_compID);
  ESPUI.addControl(ControlType::Option, "Pink", "pink", ControlColor::Peterriver, espui_led2ColorSelect_compID);

  //LED2 mode selector
  espui_led2ModeSelect_compID = ESPUI.addControl(ControlType::Select, "LED2 Mode:", "", ControlColor::Peterriver, mainTab, &espui_select_led2Mode_CALLBACK);
  ESPUI.addControl(ControlType::Option, "SOLID", "solid", ControlColor::Peterriver, espui_led2ModeSelect_compID);
  ESPUI.addControl(ControlType::Option, "FADE", "fade", ControlColor::Peterriver, espui_led2ModeSelect_compID);

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
  espui_display_brightness_compID = ESPUI.addControl(ControlType::Slider, "Display Brightness", "", ControlColor::Peterriver, confTab, &espui_slider_displayBrightness_CALLBACK);
  espui_display_sleep_compID = ESPUI.addControl(ControlType::Switcher, "Display Sleep", "", ControlColor::Peterriver, confTab, &espui_button_displaySleep_CALLBACK);
  espui_flash_save_compID = ESPUI.addControl(ControlType::Button, "Save Settings to Flash", "", ControlColor::Peterriver, confTab, &espui_button_saveToFlash_CALLBACK);

  ESPUI.begin("Enclosure Controller ESP32");
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
    int convertedValue = map(fan1_dutyCycle, 0, 100, 0, 255);
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
    String graphCommand = "add 1,0,";
    graphCommand.concat(map(temperature, dht_1_min_tempGraph + 128, 255, 128, 255));
    myNextion.sendCommand(string2char(graphCommand));
  }
  else if(sensorID == 1){
    //ESPUI.addGraphPoint(espui_sens2Graph_compID, 30);
    String graphCommand = "add 1,1,";
    graphCommand.concat(map(temperature, dht_2_min_tempGraph + 0, 255, 0, 127));
    myNextion.sendCommand(string2char(graphCommand));
  }
}
void setDisplaySleep(bool value){
  if(value == false){
      myNextion.sendCommand("thsp=0");
      myNextion.sendCommand("thup=0");
  }
  else{
    myNextion.sendCommand("thsp=10");
    myNextion.sendCommand("thup=1");
  }
  myNextion.setComponentValue("conf_page.btn_sleep", value);
  ESPUI.updateSwitcher(espui_display_sleep_compID, value);
}
void setDisplayBrightness(int value){
  ESPUI.updateSlider(espui_display_brightness_compID, value);
  myNextion.sendCommand(string2char("dims=" + String(display_brightness)));
}
void setLed2Color(String color){
  if(led2_mode_selected == 0){
    if(color == "black"){led2_color_selected = CRGB::Black;}
    else if(color == "white"){led2_color_selected = CRGB::White;}
    else if(color == "red"){led2_color_selected = CRGB::Red;}
    else if(color == "orange"){led2_color_selected = CRGB::OrangeRed;}
    else if(color == "yellow"){led2_color_selected = CRGB::Yellow;}
    else if(color == "green"){led2_color_selected = CRGB::Green;}
    else if(color == "blue"){led2_color_selected = CRGB::Blue;}
    else if(color == "purple"){led2_color_selected = CRGB::Purple;}
    else if(color == "pink"){led2_color_selected = CRGB::DeepPink;}

    ESPUI.updateSelect(espui_led2ColorSelect_compID, color);

    myNextion.setComponentValue("led_page.btn_black", 0);
    myNextion.setComponentValue("led_page.btn_white", 0);
    myNextion.setComponentValue("led_page.btn_red", 0);
    myNextion.setComponentValue("led_page.btn_orange", 0);
    myNextion.setComponentValue("led_page.btn_yellow", 0);
    myNextion.setComponentValue("led_page.btn_green", 0);
    myNextion.setComponentValue("led_page.btn_blue", 0);
    myNextion.setComponentValue("led_page.btn_purple", 0);
    myNextion.setComponentValue("led_page.btn_pink", 0);

    if(color == "black"){myNextion.setComponentValue("led_page.btn_black", 1);}
    else if(color == "white"){myNextion.setComponentValue("led_page.btn_white", 1);}
    else if(color == "red"){myNextion.setComponentValue("led_page.btn_red", 1);}
    else if(color == "orange"){myNextion.setComponentValue("led_page.btn_orange", 1);}
    else if(color == "yellow"){myNextion.setComponentValue("led_page.btn_yellow", 1);}
    else if(color == "green"){myNextion.setComponentValue("led_page.btn_green", 1);}
    else if(color == "blue"){myNextion.setComponentValue("led_page.btn_blue", 1);}
    else if(color == "purple"){myNextion.setComponentValue("led_page.btn_purple", 1);}
    else if(color == "pink"){myNextion.setComponentValue("led_page.btn_pink", 1);}

    for(int i = 0; i < led2_numberOfLEDs; i++){
      leds[i] = led2_color_selected;
    }
    FastLED.show();
  }
  
}
void setLed2Mode(String mode){
  ESPUI.updateSelect(espui_led2ModeSelect_compID, mode);
  mode.toUpperCase();
  myNextion.setComponentText("led_page.tf_mode", mode);
}
void loadFlashSave(){
  preferences.begin("enclC_E32", false);
  fan1_dutyCycle = preferences.getInt("fan1_dC", 0);
  fan2_dutyCycle = preferences.getInt("fan2_dC", 0);
  int led1_relayState = preferences.getInt("led1_rlyState", 0);
  led2_color_selected = preferences.getInt("led2_color_sel", 0);
  led2_mode_selected = preferences.getInt("led2_mode_sel", 0);
  display_brightness = preferences.getInt("disp_brt", 80);
  display_sleepState = preferences.getInt("disp_slpState", 0);
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

  if(led2_mode_selected == 0 && led2_color_selected){
    for(int i = 0; i < led2_numberOfLEDs; i++){
      leds[i] = led2_color_selected;
    }
    FastLED.show();
  }

  if(display_brightness <= 20 || display_brightness > 100){
    display_brightness = 80;
  }
  setDisplayBrightness(display_brightness);
  
  setDisplaySleep(display_sleepState);
}