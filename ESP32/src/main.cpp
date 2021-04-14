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
#include <ESPAsyncWebServer.h>
#include "task-events.h"
#include "helper.h"
#include "controller-pin-definition.h"
#include "controller-configuration.h"

#include "../lib/framework/ESP8266React.h"
#include "./libs/NextionDisplay.h"
#include "./libs/Sensor.h"
#include "./libs/Relay.h"
#include "./libs/FasterLed.h"
#include "./libs/Fan.h"
#include "./effects/EffectLoader.h"

EventGroupHandle_t eg;

AsyncWebSocket ws("/ws");
AsyncWebServer server(80);
ESP8266React esp8266React(&server);

Sensor *sensor1;
Sensor *sensor2;
Relay *led1;
FasterLed *led2;
Fan *fan1;
Fan *fan2;
EffectLoader *effectLoader;
bool booted = false;

NextionDisplay *nextion;

void onWsEvent(AsyncWebSocket *webSocket, AsyncWebSocketClient *client, AwsEventType type, void *arg, uint8_t *data,
			   size_t len);

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
	if (command == "setEffect") return setEffect;
	if (command == "setPercent") return setPercent;
	return invalid;
}

void IRAM_ATTR fan1TachoInterrupt()
{
	BaseType_t xHigherPriorityTaskWoken, xResult;
	xHigherPriorityTaskWoken = pdFALSE;

	if (fan1->halfRevolutions >= fanSenseInterval) {
		xResult = xEventGroupSetBitsFromISR(eg, TASK_EVENT_FAN1_CalcRpm, &xHigherPriorityTaskWoken);
		if (xResult != pdFAIL) {
			portYIELD_FROM_ISR();
		}
	} else {
		fan1->halfRevolutions++;
	}
}

void IRAM_ATTR fan2TachoInterrupt()
{
	BaseType_t xHigherPriorityTaskWoken, xResult;
	xHigherPriorityTaskWoken = pdFALSE;

	if (fan2->halfRevolutions >= fanSenseInterval) {
		xResult = xEventGroupSetBitsFromISR(eg, TASK_EVENT_FAN2_CalcRpm, &xHigherPriorityTaskWoken);
		if (xResult != pdFAIL) {
			portYIELD_FROM_ISR();
		}
	} else {
		fan2->halfRevolutions++;
	}
}

void sensor1TemperatureUpdated(void *params)
{
	for (;;) {
		xEventGroupWaitBits(eg, TASK_EVENT_SENSOR1_TemperatureUpdated, pdTRUE, pdTRUE, portMAX_DELAY);
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
		delay(10);
	}
}

void sensor1HumidityUpdated(void *params)
{
	for (;;) {
		xEventGroupWaitBits(eg, TASK_EVENT_SENSOR1_HumidityUpdated, pdTRUE, pdTRUE, portMAX_DELAY);
		float humidity = *((float *) params);

		String value = valueToPercentString(humidity);
		nextion->setCompText("main_page.tf_hum_sens1", value);
		nextion->setCompText("sensor_page.tf_hum_sens1", value);

		DynamicJsonDocument json(64);
		sensor1->addToJson(&json, false);
		String response;
		serializeJson(json, response);

		ws.textAll(response);
		delay(10);
	}
}

void sensor2TemperatureUpdated(void *params)
{
	for (;;) {
		xEventGroupWaitBits(eg, TASK_EVENT_SENSOR2_TemperatureUpdated, pdTRUE, pdTRUE, portMAX_DELAY);
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
		delay(10);
	}
}

void sensor2HumidityUpdated(void *params)
{
	for (;;) {
		xEventGroupWaitBits(eg, TASK_EVENT_SENSOR2_HumidityUpdated, pdTRUE, pdTRUE, portMAX_DELAY);
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
		delay(10);
	}
}

void fan1RpmUpdated(void *params)
{
	for (;;) {
		xEventGroupWaitBits(eg, TASK_EVENT_FAN1_RpmUpdated, pdTRUE, pdTRUE, portMAX_DELAY);
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
		delay(10);
	}
}

void fan1PwmUpdated(void *params)
{
	for (;;) {
		xEventGroupWaitBits(eg, TASK_EVENT_FAN1_PwmUpdated, pdTRUE, pdTRUE, portMAX_DELAY);
		//int dutyCycle = fan1->getDutyCycle();
		int percent = fan1->getPercent();

		String value = valueToPercentString(percent);
		nextion
				->setCompText("main_page.tf_pwm_fan1", value)
				->setCompText("fans_page.tf_pwm_fan1", value)
				->setCompValue("fans_page.sli_speed_fan1", percent);

		DynamicJsonDocument json(64);
		fan1->addToJson(&json, false, true, true);
		String response;
		serializeJson(json, response);

		ws.textAll(response);
		delay(10);
	}
}

void fan2RpmUpdated(void *params)
{
	for (;;) {
		xEventGroupWaitBits(eg, TASK_EVENT_FAN2_RpmUpdated, pdTRUE, pdTRUE, portMAX_DELAY);
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
		delay(10);
	}
}

void fan2PwmUpdated(void *params)
{
	for (;;) {
		xEventGroupWaitBits(eg, TASK_EVENT_FAN2_PwmUpdated, pdTRUE, pdTRUE, portMAX_DELAY);
		//int dutyCycle = fan2->getDutyCycle();
		int percent = fan2->getPercent();

		String value = valueToPercentString(percent);
		nextion
				->setCompText("main_page.tf_pwm_fan2", value)
				->setCompText("fans_page.tf_pwm_fan2", value)
				->setCompValue("fans_page.sli_speed_fan2", percent);

		DynamicJsonDocument json(64);
		fan2->addToJson(&json, false, true, true);
		String response;
		serializeJson(json, response);

		ws.textAll(response);
		delay(10);
	}
}

void led1StateUpdated(void *params)
{
	for (;;) {
		xEventGroupWaitBits(eg, TASK_EVENT_LED1_StateUpdated, pdTRUE, pdTRUE, portMAX_DELAY);
		bool state = *((bool *) params);

		nextion->setCompValue("led_page.btn_led1", state ? 1 : 0);

		DynamicJsonDocument json(128);
		led1->addToJson(&json);
		String response;
		serializeJson(json, response);

		ws.textAll(response);
		delay(10);
	}
}

void nextionCompClickedCallback(void *params)
{
	for (;;) {
		xEventGroupWaitBits(eg, TASK_EVENT_NEXTION_CompClicked, pdTRUE, pdTRUE, portMAX_DELAY);
		const int pageId = nextion->compClicked_pageId;
		const int compId = nextion->compClicked_compId;

		nextion->compClicked_pageId = -1;
		nextion->compClicked_compId = -1;

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
					case 1:
						led1->setState(nextion->getCompValue("led_page.btn_led1"));
						break;
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
}

void effectChangeCallback(void *params)
{
	for (;;) {
		xEventGroupWaitBits(eg, TASK_EVENT_LED2_EffectChanged, pdTRUE, pdTRUE, portMAX_DELAY);
		//int newEffectId = *((int *) params);

		DynamicJsonDocument json(64);
		effectLoader->addToJson(&json, true, false);
		String response;
		serializeJson(json, response);

		ws.textAll(response);
		delay(10);
	}
}

void setup()
{
	Serial.begin(serial1BaudRate);
	while (!Serial) {}
	Serial.println("3D-Print-Enclosure-Controller booting v" + esp32Version);
	delay(100);
	eg = xEventGroupCreate();

	//Temp & Humidity sensor setup
	sensor1 = new Sensor(
			"sensor1",
			dht22_1_pin,
			dhtSenseInterval,
			eg,
			TASK_EVENT_SENSOR1_TemperatureUpdated,
			TASK_EVENT_SENSOR1_HumidityUpdated,
			&sensor1TemperatureUpdated,
			&sensor1HumidityUpdated
	);
	sensor2 = new Sensor(
			"sensor2",
			dht22_2_pin,
			dhtSenseInterval,
			eg,
			TASK_EVENT_SENSOR2_TemperatureUpdated,
			TASK_EVENT_SENSOR2_HumidityUpdated,
			&sensor2TemperatureUpdated,
			&sensor2HumidityUpdated
	);

	//Led1 (relay) setup
	led1 = new Relay("led1", led1_relay_pin, eg, TASK_EVENT_LED1_StateUpdated, &led1StateUpdated);

	//Led2 (WS2812B) setup
	led2 = new FasterLed(led2_data_pin, led2NumberOfLeds, led2Brightness, led2CurrentLimit);
	FastLED.addLeds<WS2812B, led2_data_pin, GRB>(led2->leds, led2NumberOfLeds);
	led2->begin();
	effectLoader = new EffectLoader(led2, eg, TASK_EVENT_LED2_EffectChanged, &effectChangeCallback);

	//Buzzer
	pinMode(buzzer_pin, OUTPUT);

	//Fan
	fan1 = new Fan(
			"fan1",
			0,
			eg,
			TASK_EVENT_FAN1_CalcRpm,
			TASK_EVENT_FAN1_RpmUpdated,
			TASK_EVENT_FAN1_PwmUpdated,
			fan1_tacho_pin,
			fan1_pwm_pin,
			&fan1RpmUpdated,
			&fan1PwmUpdated
	);
	fan2 = new Fan(
			"fan2",
			1,
			eg,
			TASK_EVENT_FAN2_CalcRpm,
			TASK_EVENT_FAN2_RpmUpdated,
			TASK_EVENT_FAN2_PwmUpdated,
			fan2_tacho_pin,
			fan2_pwm_pin,
			&fan2RpmUpdated,
			&fan2PwmUpdated
	);

	//Display
	nextion = new NextionDisplay(displayBootDelay, serial2BaudRate, nextionDisplayRX, nextionDisplayTX, eg,
								 TASK_EVENT_NEXTION_CompClicked, &nextionCompClickedCallback);

	String version = "ESP32: v";
	version.concat(esp32Version);
	nextion->setCompText("about_page.tf_esp32_v", version);

	esp8266React.begin();
	ws.onEvent(onWsEvent);

	server.addHandler(&ws);
	server.begin();

	//Delay and attaching interrupt after everything is
	//required to prevent crashes from happening on boot caused by weird interrupt stuff

	attachInterrupt(digitalPinToInterrupt(fan1_tacho_pin), fan1TachoInterrupt, FALLING);
	attachInterrupt(digitalPinToInterrupt(fan2_tacho_pin), fan2TachoInterrupt, FALLING);

	Serial.println("3D-Print-Enclosure-Controller booted");
	booted = true;
	fan1->setPercent(100);
	fan2->setPercent(100);
}

DynamicJsonDocument handleWebSocketCommunication(String _component, String _command, String value)
{
	DynamicJsonDocument responseDoc(512);
	WebSocketComponent component = resolveWebSocketComponent(_component);
	WebSocketCommand command = resolveWebSocketCommand(_command);

	if (command == WebSocketCommand::invalid) {
		responseDoc["message"] = "Invalid command";
		responseDoc["status"] = "failure";
		responseDoc["command"] = command;
		return responseDoc;
	}

	switch (component) {
		case Led1:
			if (command == setState) {
				int newState = value.toInt();
				led1->setState(newState);
			}
			break;
		case Led2:
			if (command == setEffect) {
				int newEffect = value.toInt();
				effectLoader->changeEffect(newEffect);
			}
			break;
		case Fan1:
		case Fan2:
			if(command == setPercent) {
				auto fan = component == Fan1 ? fan1 : fan2;
				int newPercent = value.toInt();
				fan->setPercent(newPercent);
				break;
			}
		case Invalid:
		default:
			responseDoc["message"] = "Invalid component";
			responseDoc["status"] = "failure";
			responseDoc["component"] = component;
			return responseDoc;
	}

	responseDoc["message"] = "Executed command";
	responseDoc["status"] = "success";
	responseDoc["command"] = _command;
	responseDoc["component"] = _component;

	return responseDoc;
}

void onWsEvent(AsyncWebSocket *webSocket, AsyncWebSocketClient *client, AwsEventType type, void *arg, uint8_t *data,
			   size_t len)
{

	if (type == WS_EVT_CONNECT) {
		client->printf("Hello Client %u :)", client->id());
		DynamicJsonDocument json(1024);
		led1->addToJson(&json);
		effectLoader->addToJson(&json);
		fan1->addToJson(&json);
		fan2->addToJson(&json);
		sensor1->addToJson(&json);
		sensor2->addToJson(&json);
		String response;
		serializeJson(json, response);
		client->text(response);
	} else if (type == WS_EVT_DATA) {
		AwsFrameInfo *info = (AwsFrameInfo *) arg;

		if (info->final && info->index == 0 && info->len == len && info->opcode == WS_TEXT) {
			/*
			Serial.printf("ws[%s][%u] %s-message[%llu]: ", webSocket->url(), client->id(),
						  (info->opcode == WS_TEXT) ? "text" : "binary", info->len);
*/
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
			}

			DynamicJsonDocument responseDoc = handleWebSocketCommunication(json["component"], json["command"],
																		   json["value"]);
			String response;
			serializeJson(responseDoc, response);
			client->text(response);
		} else {
			DynamicJsonDocument doc(192);
			doc["reason"] = "Only single frame as a json string allowed for communication";
			doc["message"] = "Error in communication";
			doc["status"] = "failure";
			String response;
			serializeJson(doc, response);
			client->text(response);
		}
	}
}

void loop()
{
	esp8266React.loop();

	Serial.printf("Free heap: %d\n", ESP.getFreeHeap());
	delay(500);
}