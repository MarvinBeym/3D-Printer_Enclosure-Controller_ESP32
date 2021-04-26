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
				int _tempWarnUpdateEvent,
				void (*temperatureUpdateCallback)(void *),
				void (*humidityUpdateCallback)(void *),
				void (*tempWarnUpdateCallback)(void *)
		);
		void addToJson(DynamicJsonDocument *doc, bool includeTemperature = true, bool includeHumidity = true, bool includeTempWarn = true) const;
		const char *name;
		void setTempWarnEnabled(bool enabled);
		void setTempDangerEnabled(bool enabled);
		void setTempWarnThreshold(int threshold);
		void setTempDangerThreshold(int threshold);
		bool getTempWarnEnabled();
		bool getTempDangerEnabled();
		int getTempWarnThreshold();
		int getTempDangerThreshold();
		float getTemperature();
	private:
		void (*temperatureUpdateCallback)(void *);
		void (*humidityUpdateCallback)(void *);
		static void taskHandler(void *parameter);
		void readSensorTask();
		int temperatureUpdateEvent;
		int humidityUpdateEvent;
		int tempWarnUpdateEvent;
		EventGroupHandle_t eg;
		DHT *dht;
		int senseInterval;
		float temperature;
		float humidity;
		bool tempWarnEnabled = false;
		bool tempDangerEnabled = false;
		int tempWarnThreshold = 100;
		int tempDangerThreshold = 100;
};