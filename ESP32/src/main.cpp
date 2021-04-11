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
#include "task-events.h"
#include "helper.h"
#include "controller-pin-definition.h"
#include "controller-configuration.h"
#include "./libs/Sensor.h"
#include "./libs/Relay.h"
#include "./libs/FasterLed.h"
#include "./libs/Fan.h"
#include "./effects/EffectLoader.h"

#include <ESPAsyncWebServer.h>
#include "../lib/framework/ESP8266React.h"

#define TASK_TEST_BIT 1
EventGroupHandle_t eg;
/*
AsyncWebSocket ws("/ws");
AsyncWebServer server(80);
ESP8266React esp8266React(&server);
*/
Sensor *sensor1;
Sensor *sensor2;
Relay *led1;
FasterLed *led2;
Fan *fan1;
Fan *fan2;
EffectLoader *effectLoader;
bool booted = false;

#include "./libs/NextionDisplay.h"

NextionDisplay *nextion;
/*
void onWsEvent(AsyncWebSocket *webSocket, AsyncWebSocketClient *client, AwsEventType type, void *arg, uint8_t *data,
			   size_t len);
*/
void HandleDisplayPage(void *parameter);

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
		//ws.textAll(response);
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
		//ws.textAll(response);
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
		//ws.textAll(response);
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
		//ws.textAll(response);
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
		//ws.textAll(response);
	}
}

void fan1PwmUpdated(void *params) {
	for(;;) {
		xEventGroupWaitBits(eg, TASK_EVENT_FAN1_PwmUpdated, pdTRUE, pdTRUE, portMAX_DELAY);
		int dutyCycle = fan1->getDutyCycle();
		int percent = fan1->getPercent();
		vTaskDelay(10);
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
		//ws.textAll(response);
	}
}

void fan2PwmUpdated(void *params) {
	for(;;) {
		xEventGroupWaitBits(eg, TASK_EVENT_FAN2_PwmUpdated, pdTRUE, pdTRUE, portMAX_DELAY);
		int dutyCycle = fan2->getDutyCycle();
		int percent = fan2->getPercent();
		vTaskDelay(10);
	}
}

void led1StateUpdated(void *params)
{
	for (;;) {
		xEventGroupWaitBits(eg, TASK_EVENT_LED1_StateUpdated, pdTRUE, pdTRUE, portMAX_DELAY);
		bool state = *((bool *) params);
		delay(20);
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
	effectLoader = new EffectLoader(led2);
	effectLoader->changeEffect(1);

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

	//xTaskCreate(HandleDisplayPage, "handleDisplayPage", 5000, nullptr, 2, nullptr);
/*
	esp8266React.begin();
	ws.onEvent(onWsEvent);

	server.addHandler(&ws);
	server.begin();
*/
	//Delay and attaching interrupt after everything is
	//required to prevent crashes from happening on boot caused by weird interrupt stuff

	attachInterrupt(digitalPinToInterrupt(fan1_tacho_pin), fan1TachoInterrupt, FALLING);
	attachInterrupt(digitalPinToInterrupt(fan2_tacho_pin), fan2TachoInterrupt, FALLING);

	Serial.println("3D-Print-Enclosure-Controller booted");
	booted = true;
	fan1->setPercent(100);
	fan2->setPercent(100);
}

void onWsEvent(AsyncWebSocket *webSocket, AsyncWebSocketClient *client, AwsEventType type, void *arg, uint8_t *data,
			   size_t len)
{

	if (type == WS_EVT_CONNECT) {
		Serial.printf("ws[%s][%u] connect\n", webSocket->url(), client->id());
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
	}
}
/*
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
	}
}
*/
void loop()
{
	//esp8266React.loop();

	Serial.printf("Free heap: %d\n", ESP.getFreeHeap());
	delay(500);
}