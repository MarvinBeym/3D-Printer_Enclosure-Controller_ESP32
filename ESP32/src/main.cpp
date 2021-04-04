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
#include <Arduino.h>
#include <soc/timer_group_struct.h>
#include <soc/timer_group_reg.h>

//Own classes
#include "Sensor.h"
#include "PinDefinition.h"
#include "Configuration.h"
#include "Relay.h"
#include "FasterLed.h"
#include "Fan.h"
#include "NextionDisplay.h"

const String esp32Version = "2.0.0";
Sensor *sensor1;
Sensor *sensor2;
Relay *led1;
FasterLed *led2;
Fan *fan1;
Fan *fan2;
NextionDisplay *nextionDisplay;

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
	while (!Serial) {
	}
	Serial.println("3D-Print-Enclosure-Controller booting v" + esp32Version);
	Serial2.begin(serial2BaudRate, SERIAL_8N1, nextionDisplayRX, nextionDisplayTX);
	while (!Serial2) {
	}
	delay(300);

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

	//Buzzer
	pinMode(buzzer_pin, OUTPUT);

	//Fan
	fan1 = new Fan("fan1", 0, fan1_tacho_pin, fan1_pwm_pin);
	fan2 = new Fan("fan2", 1, fan2_tacho_pin, fan2_pwm_pin);
	attachInterrupt(digitalPinToInterrupt(fan1_tacho_pin), fan1TachoInterrupt, FALLING);
	attachInterrupt(digitalPinToInterrupt(fan2_tacho_pin), fan2TachoInterrupt, FALLING);

	fan1->begin();
	fan2->begin();

	//Display
	nextionDisplay = new NextionDisplay(Serial2, serial2BaudRate);
	nextionDisplay->begin(displayBootDelay);

	xTaskCreate(HandleDisplayPage, "handleDisplayPage", 5000, nullptr, 2, nullptr);

	fan1->setSpeed(50);
	fan2->setSpeed(80);
	Serial.println("3D-Print-Enclosure-Controller booted");
}

void HandleDisplayPageChanging(int pageId, int compId)
{
	switch (pageId) {
		case MAIN_PAGE:
			switch (compId) {
				case 1:
					nextionDisplay->setPage(FANS_PAGE);
					break;
				case 2:
					nextionDisplay->setPage(SENSOR_PAGE);
					break;
				case 3:
					nextionDisplay->setPage(LED_PAGE);
					break;
				case 4:
					nextionDisplay->setPage(CONF_PAGE);
			}
			break;
		case FANS_PAGE:
			switch (compId) {
				case 6:
					nextionDisplay->setPage(MAIN_PAGE);
					break;
			}
			break;
		case SENSOR_PAGE:
			switch (compId) {
				case 2:
					nextionDisplay->setPage(MAIN_PAGE);
					break;
			}
			break;
		case LED_PAGE:
			switch (compId) {
				case 14:
					nextionDisplay->setPage(MAIN_PAGE);
					break;
			}
			break;
		case CONF_PAGE:
			switch (compId) {
				case 1:
					nextionDisplay->setPage(MAIN_PAGE);
					break;
				case 2:
					nextionDisplay->setPage(ABOUT_PAGE);
					break;
			}
			break;
		case ABOUT_PAGE:
			switch (compId) {
				case 6:
					nextionDisplay->setPage(CONF_PAGE);
					break;
			}
			break;
	}
}

void HandleDisplayPage(void *parameter)
{
	for (;;) {
		switch (nextionDisplay->pageId) {
			case MAIN_PAGE:
				nextionDisplay->setCompText("main_page.tf_speed_fan1", String(fan1->rpm) + "rpm");
				nextionDisplay->setCompText("main_page.tf_pwm_fan1", String(fan1->speed) + "%");

				nextionDisplay->setCompText("main_page.tf_speed_fan2", String(fan2->rpm) + "rpm");
				nextionDisplay->setCompText("main_page.tf_pwm_fan2", String(fan2->speed) + "%");

				nextionDisplay->setCompText("main_page.tf_temp_sens1", String(sensor1->temperature) + (char) 176 + "C");
				nextionDisplay->setCompText("main_page.tf_hum_sens1", String(sensor1->humidity) + "%");

				nextionDisplay->setCompText("main_page.tf_temp_sens2", String(sensor2->temperature) + (char) 176 + "C");
				nextionDisplay->setCompText("main_page.tf_hum_sens2", String(sensor2->humidity) + "%");
				break;
			case FANS_PAGE:

				break;
			case SENSOR_PAGE:

				break;
			case LED_PAGE:

				break;
			case CONF_PAGE:

				break;
			case ABOUT_PAGE:
				String version = "ESP32: v";
				version.concat(esp32Version);
				nextionDisplay->setCompText("about_page.tf_esp32_v", version);
				break;
		}

		Serial.println(uxTaskGetStackHighWaterMark(nullptr));
		vTaskDelay(pdMS_TO_TICKS(displayPageRefreshInterval));
	}
}

void loop()
{
	//Prevents the esp32 from crashing (watchdog not getting feed)
	//This is required as code is split into tasks and no code is run inside the loop
	TIMERG0.wdt_wprotect = TIMG_WDT_WKEY_VALUE;
	TIMERG0.wdt_feed = 1;
	TIMERG0.wdt_wprotect = 0;

	int pageId = -1;
	int compId = -1;
	nextionDisplay->getComponentClicked(pageId, compId);
	if (pageId != -1 && compId != -1) {
		Serial.print(pageId);
		Serial.print(" | ");
		Serial.println(compId);
		HandleDisplayPageChanging(pageId, compId);
	}
}