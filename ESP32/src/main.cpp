/**
 * 3D-Printer Enclosure Controller firmware for the ESP32
 * Copyright (C) 2021  Marvin Beym
 * 
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 * 
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <https://www.gnu.org/licenses/>.
 **/

#define FASTLED_INTERNAL

#include <Arduino.h>
#include <soc/timer_group_struct.h>
#include <soc/timer_group_reg.h>

#include "helper.h"
#include "controller-pin-definition.h"
#include "controller-configuration.h"
#include "./libs/Sensor.h"
#include "./libs/Relay.h"
#include "./libs/FasterLed.h"
#include "./libs/Fan.h"
#include "./effects/EffectLoader.h"

#ifdef WEBINTERFACE_ENABLED

#include <ESPAsyncWebServer.h>
#include "../lib/framework/ESP8266React.h"

AsyncWebSocket ws("/ws");
AsyncWebServer server(80);
ESP8266React esp8266React(&server);
#endif

Sensor *sensor1;
Sensor *sensor2;
Relay *led1;
FasterLed *led2;
Fan *fan1;
Fan *fan2;
EffectLoader *effectLoader;
bool booted = false;
#ifdef NEXTION_DISPLAY_ENABLED

#include "./libs/NextionDisplay.h"

NextionDisplay *nextion;
#endif

void onWsEvent(AsyncWebSocket *webSocket, AsyncWebSocketClient *client, AwsEventType type, void *arg, uint8_t *data,
			   size_t len);
void HandleDisplayPage(void *parameter);

void IRAM_ATTR fan1TachoInterrupt()
{
	fan1->incrementHalfRevolution();
}

void IRAM_ATTR fan2TachoInterrupt()
{
	fan2->incrementHalfRevolution();
}

void sensor1TemperatureUpdated(void *params)
{
	if(!booted) vTaskDelete(NULL);
	float temperature = *((float *) params);
	String value = valueToTempString(temperature);
	nextion->setCompText("main_page.tf_temp_sens1", value);
	nextion->setCompText("sensor_page.tf_temp_sens1", value);
	nextion->addGraphValue(
			1, 0,
			map((int) temperature, displayGraphMinTemp, 255, 0, displayGraphHeight));
	DynamicJsonDocument json(64);
	sensor1->addToJson(&json, true, false);
	String response;
	serializeJson(json, response);
	ws.textAll(response);
	vTaskDelete(NULL);
}

void sensor1HumidityUpdated(void *params)
{
	if(!booted) vTaskDelete(NULL);
	float humidity = *((float *) params);
	String value = valueToPercentString(humidity);
	nextion->setCompText("main_page.tf_hum_sens1", value);
	nextion->setCompText("sensor_page.tf_hum_sens1", value);

	DynamicJsonDocument json(64);
	sensor1->addToJson(&json, false);
	String response;
	serializeJson(json, response);
	ws.textAll(response);
	vTaskDelete(NULL);
}

void sensor2TemperatureUpdated(void *params)
{
	if(!booted) vTaskDelete(NULL);
	float temperature = *((float *) params);
	String value = valueToTempString(temperature);
	nextion
			->setCompText("main_page.tf_temp_sens2", value)
			->setCompText("sensor_page.tf_temp_sens2", value)
			->addGraphValue(
					1, 1,
					map((int) temperature, displayGraphMinTemp, 255, 0, displayGraphHeight));
	DynamicJsonDocument json(64);
	sensor2->addToJson(&json, true, false);
	String response;
	serializeJson(json, response);
	ws.textAll(response);
	vTaskDelete(NULL);
}

void sensor2HumidityUpdated(void *params)
{
	if(!booted) vTaskDelete(NULL);
	float humidity = *((float *) params);
	String value = valueToPercentString(humidity);
	nextion
			->setCompText("main_page.tf_hum_sens2", value)
			->setCompText("sensor_page.tf_hum_sens2", value);

	DynamicJsonDocument json(64);
	sensor2->addToJson(&json, false);
	String response;
	serializeJson(json, response);
	ws.textAll(response);
	vTaskDelete(NULL);
}

void fan1RpmUpdated(void *params)
{
	if(!booted) vTaskDelete(NULL);
	int rpm = *((int *) params);

	String value = valueToRpmString(rpm);
	nextion
			->setCompText("main_page.tf_speed_fan1", value)
			->setCompText("fans_page.tf_speed_fan1", value);

	DynamicJsonDocument json(64);
	fan1->addToJson(&json, true, false, false);
	String response;
	serializeJson(json, response);
	ws.textAll(response);
	vTaskDelete(NULL);
}

void fan2RpmUpdated(void *params)
{
	if(!booted) vTaskDelete(NULL);
	int rpm = *((int *) params);

	String value = valueToRpmString(rpm);
	nextion
			->setCompText("main_page.tf_speed_fan2", value)
			->setCompText("fans_page.tf_speed_fan2", value);

	DynamicJsonDocument json(64);
	fan2->addToJson(&json, true, false, false);
	String response;
	serializeJson(json, response);
	ws.textAll(response);
	vTaskDelete(NULL);
}

void setup()
{
	Serial.begin(serial1BaudRate);
	while (!Serial) {}
	Serial.println("3D-Print-Enclosure-Controller booting v" + esp32Version);
	delay(100);

	//Temp & Humidity sensor setup
	sensor1 = new Sensor("sensor1", dht22_1_pin, dhtSenseInterval, &sensor1TemperatureUpdated, &sensor1HumidityUpdated);
	sensor2 = new Sensor("sensor2", dht22_2_pin, dhtSenseInterval, &sensor2TemperatureUpdated, &sensor2HumidityUpdated);
	sensor1->begin();
	sensor2->begin();

	//Led1 (relay) setup
	led1 = new Relay(led1_relay_pin, false);
	led1->begin();

	//Led2 (WS2812B) setup
	led2 = new FasterLed(led2_data_pin, led2NumberOfLeds, led2Brightness, led2CurrentLimit);
	FastLED.addLeds<WS2812B, led2_data_pin, GRB>(led2->leds, led2NumberOfLeds);
	led2->begin();
	effectLoader = new EffectLoader(led2);
	effectLoader->changeEffect(1);

	//Buzzer
	pinMode(buzzer_pin, OUTPUT);

	//Fan
	fan1 = new Fan("fan1", 0, fan1_tacho_pin, fan1_pwm_pin, &fan1RpmUpdated);
	fan2 = new Fan("fan2", 1, fan2_tacho_pin, fan2_pwm_pin, &fan2RpmUpdated);
	fan1->begin();
	fan2->begin();

#ifdef NEXTION_DISPLAY_ENABLED
	//Display
	nextion = new NextionDisplay(serial2BaudRate, nextionDisplayRX, nextionDisplayTX);
	nextion->begin(displayBootDelay);

	xTaskCreate(HandleDisplayPage, "handleDisplayPage", 5000, nullptr, 2, nullptr);
#endif

#ifdef WEBINTERFACE_ENABLED
	esp8266React.begin();
	ws.onEvent(onWsEvent);

	server.addHandler(&ws);
	server.begin();

	//Delay and attaching interrupt after everything is
	//required to prevent crashes from happening on boot caused by weird interrupt stuff
	delay(2000);
#endif

	attachInterrupt(digitalPinToInterrupt(fan1_tacho_pin), fan1TachoInterrupt, FALLING);
	attachInterrupt(digitalPinToInterrupt(fan2_tacho_pin), fan2TachoInterrupt, FALLING);

	Serial.println("3D-Print-Enclosure-Controller booted");
	//booted = true;
}

void onWsEvent(AsyncWebSocket *webSocket, AsyncWebSocketClient *client, AwsEventType type, void *arg, uint8_t *data,
			   size_t len)
{

	if (type == WS_EVT_CONNECT) {
		Serial.printf("ws[%s][%u] connect\n", webSocket->url(), client->id());
		client->printf("Hello Client %u :)", client->id());
		DynamicJsonDocument json(1024);
		JsonObject led1Json = json.createNestedObject("led1");
		led1Json["state"] = led1->getState();
		effectLoader->addToJson(&json);
		fan1->addToJson(&json);
		fan2->addToJson(&json);
		sensor1->addToJson(&json);
		sensor2->addToJson(&json);
		String response;
		serializeJson(json, response);
		client->text(response);
	}
}

#ifdef NEXTION_DISPLAY_ENABLED

void HandleDisplayInteraction(int pageId, int compId)
{
	switch (pageId) {
		case MAIN_PAGE:
			switch (compId) {
				case 1:
					nextion->setPage(FANS_PAGE);
					break;
				case 2:
					nextion->setPage(SENSOR_PAGE);
					break;
				case 3:
					nextion->setPage(LED_PAGE);
					break;
				case 4:
					nextion->setPage(CONF_PAGE);
			}
			break;
		case FANS_PAGE:
			switch (compId) {
				case 3:
					delay(20);
					fan2->setPercent(nextion->getCompValue("fans_page.sli_speed_fan2"));
					delay(20);

					break;
				case 4:
					delay(20);
					fan1->setPercent(nextion->getCompValue("fans_page.sli_speed_fan1"));
					delay(20);
					break;
				case 6:
					nextion->setPage(MAIN_PAGE);
					break;
			}
			break;
		case SENSOR_PAGE:
			switch (compId) {
				case 2:
					nextion->setPage(MAIN_PAGE);
					break;
			}
			break;
		case LED_PAGE:
			switch (compId) {
				case 14:
					nextion->setPage(MAIN_PAGE);
					break;
			}
			break;
		case CONF_PAGE:
			switch (compId) {
				case 1:
					nextion->setPage(MAIN_PAGE);
					break;
				case 2:
					nextion->setPage(ABOUT_PAGE);
					break;
			}
			break;
		case ABOUT_PAGE:
			switch (compId) {
				case 6:
					nextion->setPage(CONF_PAGE);
					break;
			}
			break;
	}
}

void HandleDisplayPage(void *parameter)
{
	for (;;) {
		switch (nextion->pageId) {
			case MAIN_PAGE:
				nextion
						->setCompText("main_page.tf_pwm_fan1", valueToPercentString(fan1->percent))
						->setCompText("main_page.tf_pwm_fan2", valueToPercentString(fan2->percent));
				break;
			case FANS_PAGE:
				nextion
						->setCompText("fans_page.tf_pwm_fan1", valueToPercentString(fan1->percent))
						->setCompText("fans_page.tf_pwm_fan2", valueToPercentString(fan2->percent));
				break;
			case SENSOR_PAGE:
				break;
			case LED_PAGE:
			case CONF_PAGE:
				break;
			case ABOUT_PAGE:
				String version = "ESP32: v";
				version.concat(esp32Version);
				nextion->setCompText("about_page.tf_esp32_v", version);
				break;
		}
		vTaskDelay(pdMS_TO_TICKS(displayPageRefreshInterval));
	}
}

#endif

void loop()
{
	//Prevents the esp32 from crashing (watchdog not getting feed)
	//This is required as code is split into tasks and no code is run inside the loop
	TIMERG0.wdt_wprotect = TIMG_WDT_WKEY_VALUE;
	TIMERG0.wdt_feed = 1;
	TIMERG0.wdt_wprotect = 0;

#ifdef NEXTION_DISPLAY_ENABLED
	int pageId = -1;
	int compId = -1;
	nextion->getComponentClicked(pageId, compId);
	if (pageId != -1 && compId != -1) {
		Serial.print(pageId);
		Serial.print(" | ");
		Serial.println(compId);
		HandleDisplayInteraction(pageId, compId);
	}

#endif

#ifdef WEBINTERFACE_ENABLED
	esp8266React.loop();

	static unsigned long lastT = 0;
	unsigned long t = millis();
	if (t - lastT >= 5000) {
		Serial.printf("Free heap: %d\n", ESP.getFreeHeap());
		lastT = t;
	}
#endif
}