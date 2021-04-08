/*
  A Class for handling dht22 sensors
  Created by Marvin Beym, 02.04.2021.
*/
#ifndef Sensor_h
#define Sensor_h

#include <Arduino.h>
#include <Adafruit_Sensor.h>
#include <DHT.h>
#include <ArduinoJson.h>

class Sensor
{
	public:
		Sensor(
				char *_name,
				int pin,
				int _senseInterval,
				void (*temperatureUpdateCallback)(void *),
				void (*humidityUpdateCallback)(void *)
		);
		void begin();
		void addToJson(DynamicJsonDocument *doc, bool includeTemperature = true, bool includeHumidity = true) const;
		char *name;
	private:
		void (*temperatureUpdateCallback)(void *);
		void (*humidityUpdateCallback)(void *);
		static void taskHandler(void *parameter);
		void taskRunner();
		DHT *dht;
		int senseInterval;
		float temperature;
		float humidity;
};

#endif