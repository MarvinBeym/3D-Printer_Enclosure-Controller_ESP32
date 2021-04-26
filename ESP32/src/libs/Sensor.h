/*
  A Class for handling dht22 sensors
  Created by Marvin Beym, 02.04.2021.
*/
#pragma once

#include <Arduino.h>
#include <Adafruit_Sensor.h>
#include <DHT.h>
#include <ArduinoJson.h>

class Sensor
{
	public:
		Sensor(
				const char *_name,
				int pin,
				int _senseInterval,
				EventGroupHandle_t _eg,
				int _temperatureUpdateEvent,
				int _humidityUpdateEvent,
				int _tempDangerUpdateEvent,
				void (*temperatureUpdateCallback)(void *),
				void (*humidityUpdateCallback)(void *),
				void (*tempDangerUpdateCallback)(void *)
		);
		void addToJson(DynamicJsonDocument *doc, bool includeTemperature = true, bool includeHumidity = true, bool includeTempDanger = true) const;
		const char *name;
		void setTempDangerEnabled(bool enabled);
		void setTempDangerThreshold(int threshold);
		bool getTempDangerEnabled();
		int getTempDangerThreshold();
		float getTemperature();
	private:
		void (*temperatureUpdateCallback)(void *);
		void (*humidityUpdateCallback)(void *);
		static void taskHandler(void *parameter);
		void readSensorTask();
		int temperatureUpdateEvent;
		int humidityUpdateEvent;
		int tempDangerUpdateEvent;
		EventGroupHandle_t eg;
		DHT *dht;
		int senseInterval;
		float temperature;
		float humidity;
		bool tempDangerEnabled = false;
		int tempDangerThreshold = 100;
};