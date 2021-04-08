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


void setup()
{
	Serial.begin(serial1BaudRate);
	while (!Serial) {}
	Serial.println("3D-Print-Enclosure-Controller booting v" + esp32Version);
	delay(100);

	//Temp & Humidity sensor setup
	sensor1 = new Sensor("sensor1", dht22_1_pin, dhtSenseInterval);
	sensor2 = new Sensor("sensor2", dht22_2_pin, dhtSenseInterval);
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
	fan1 = new Fan("fan1", 0, fan1_tacho_pin, fan1_pwm_pin);
	fan2 = new Fan("fan2", 1, fan2_tacho_pin, fan2_pwm_pin);
	fan1->begin();
	fan2->begin();

#ifdef NEXTION_DISPLAY_ENABLED
	//Display
	nextion = new NextionDisplay(serial2BaudRate, nextionDisplayRX, nextionDisplayTX);
	nextion->begin(displayBootDelay);

	xTaskCreate(HandleDisplayPage, "handleDisplayPage", 5000, nullptr, 2, nullptr);
#endif

	fan1->setPercent(50);
	fan2->setPercent(80);

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
}

enum WebSocketComponent
{
	Led1,
	Led2,
	Fan1,
	Fan2,
	Invalid,
};

enum WebSocketCommand
{
	setState,
	getEffects,
	setEffect,
	setPercent,
	invalid,
};

WebSocketComponent resolveWebSocketComponent(String component)
{
	if (component == "led1") return Led1;
	if (component == "led2") return Led2;
	if (component == "fan1") return Fan1;
	if (component == "fan2") return Fan2;
	return Invalid;
}

WebSocketCommand resolveWebSocketCommand(String command)
{
	if (command == "setState") return setState;
	if (command == "getEffects") return getEffects;
	if (command == "setEffect") return setEffect;
	if (command == "setPercent") return setPercent;
	return invalid;
}


#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wswitch"

DynamicJsonDocument handleWebSocketCommunication(String _component, String _command, String value)
{
	DynamicJsonDocument responseDoc(512);
	JsonObject data = responseDoc.createNestedObject("data");
	WebSocketComponent component = resolveWebSocketComponent(_component);
	WebSocketCommand command = resolveWebSocketCommand(_command);

	if (component == WebSocketComponent::Invalid) {
		responseDoc["message"] = "Invalid component";
		responseDoc["status"] = "failure";
		responseDoc["component"] = component;
		return responseDoc;
	}
	if (command == WebSocketCommand::invalid) {
		responseDoc["message"] = "Invalid command";
		responseDoc["status"] = "failure";
		responseDoc["command"] = command;
		return responseDoc;
	}

	switch (component) {
		case Led1:
			if (command == setState) {
				JsonObject led1Json = data.createNestedObject(_component);
				bool newState = value.toInt() != 0;
				newState ? led1->on() : led1->off();
				led1Json["state"] = newState;
			}
			break;
		case Led2:
			if (command == getEffects) {
				JsonObject led2Json = data.createNestedObject(_component);
				JsonArray jsonArray = led2Json.createNestedArray("effects");
				for (std::size_t i = 0; i < effectLoader->effects.size(); ++i) {
					auto effect = effectLoader->effects[i];
					JsonObject effectObject = jsonArray.createNestedObject();
					effectObject["name"] = effect->getName();
					effectObject["id"] = effect->getEffectId();
				}
			} else if (command == setEffect) {
				int newEffect = value.toInt();
				JsonObject led2Json = data.createNestedObject(_component);
				effectLoader->changeEffect(newEffect);
				led2Json["currentEffect"] = effectLoader->getCurrentEffect();
			}
			break;
		case Fan1:
		case Fan2:
			if(command == setPercent) {
				auto fan = component == Fan1 ? fan1 : fan2;
				JsonObject fanJson = data.createNestedObject(_component);
				int newPercent = value.toInt();
				fan->setPercent(newPercent);
				fanJson["percent"] = fan->percent;
				break;
			}
	}

	responseDoc["message"] = "Executed command";
	responseDoc["status"] = "success";
	responseDoc["command"] = command;
	responseDoc["component"] = component;

	return responseDoc;
}

#pragma clang diagnostic pop

void onWsEvent(AsyncWebSocket *webSocket, AsyncWebSocketClient *client, AwsEventType type, void *arg, uint8_t *data,
			   size_t len)
{

	if(type == WS_EVT_CONNECT) {
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
		Serial.println(response);
	}

	/*
	AwsFrameInfo *info = (AwsFrameInfo *) arg;

			if (info->final && info->index == 0 && info->len == len && info->opcode == WS_TEXT) {
				Serial.printf("ws[%s][%u] %s-message[%llu]: ", webSocket->url(), client->id(),
							  (info->opcode == WS_TEXT) ? "text" : "binary", info->len);

				char *jsonString = (char *) data;
				DynamicJsonDocument json(1024);
				DeserializationError error = deserializeJson(json, jsonString);

				if (error || !json.containsKey("component") || !json.containsKey("command") ||
					!json.containsKey("value")) {
					DynamicJsonDocument errorDoc(128);
					errorDoc["reason"] = "No json, invalid json or other error";
					errorDoc["message"] = "Error in communication";
					errorDoc["status"] = "failure";
					String response;
					serializeJson(errorDoc, response);
					client->text(response);
					Serial.println(response);
					break;
				}

				Serial.printf("%s\n", jsonString);

				DynamicJsonDocument responseDoc = handleWebSocketCommunication(json["component"], json["command"],
																			   json["value"]);
				String response;
				serializeJson(responseDoc, response);
				client->text(response);
				Serial.println(response);
			} else {
				DynamicJsonDocument doc(192);
				doc["reason"] = "Only single frame as a json string allowed for communication";
				doc["message"] = "Error in communication";
				doc["status"] = "failure";
				String response;
				serializeJson(doc, response);
				client->text(response);
			}
			break;
	 */
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
#ifdef ALWAYS_UPDATE_DISPLAY_GRAPH
		nextion
				->addGraphValue(1, 0,
								map((int) sensor1->temperature, displayGraphMinTemp, 255, 0, displayGraphHeight))
				->addGraphValue(1, 1,
								map((int) sensor2->temperature, displayGraphMinTemp, 255, 0, displayGraphHeight));
#endif
		switch (nextion->pageId) {
			case MAIN_PAGE:
				nextion
						->setCompText("main_page.tf_speed_fan1", valueToRpmString(fan1->rpm))
						->setCompText("main_page.tf_pwm_fan1", valueToPercentString(fan1->percent))
						->setCompText("main_page.tf_speed_fan2", valueToRpmString(fan2->rpm))
						->setCompText("main_page.tf_pwm_fan2", valueToPercentString(fan2->percent))
						->setCompText("main_page.tf_temp_sens1", valueToTempString(sensor1->temperature))
						->setCompText("main_page.tf_hum_sens1", valueToPercentString(sensor1->humidity))
						->setCompText("main_page.tf_temp_sens2", valueToTempString(sensor2->temperature))
						->setCompText("main_page.tf_hum_sens2", valueToPercentString(sensor2->humidity));
				break;
			case FANS_PAGE:
				nextion
						->setCompText("fans_page.tf_speed_fan1", valueToRpmString(fan1->rpm))
						->setCompText("fans_page.tf_pwm_fan1", valueToPercentString(fan1->percent))
						->setCompText("fans_page.tf_speed_fan2", valueToRpmString(fan2->rpm))
						->setCompText("fans_page.tf_pwm_fan2", valueToPercentString(fan2->percent));
				break;
			case SENSOR_PAGE:
				nextion
						->setCompText("sensor_page.tf_temp_sens1", valueToTempString(sensor1->temperature))
						->setCompText("sensor_page.tf_hum_sens1", valueToPercentString(sensor1->humidity))
						->setCompText("sensor_page.tf_temp_sens2", valueToTempString(sensor2->temperature))
						->setCompText("sensor_page.tf_hum_sens2", valueToPercentString(sensor2->humidity));
#ifndef ALWAYS_UPDATE_DISPLAY_GRAPH
				nextion
						->addGraphValue(1, 0, map((int) sensor1->temperature, displayGraphMinTemp, 255, 0,
												  displayGraphHeight))
						->addGraphValue(1, 1, map((int) sensor2->temperature, displayGraphMinTemp, 255, 0,
												  displayGraphHeight));
#endif
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
#endif
}