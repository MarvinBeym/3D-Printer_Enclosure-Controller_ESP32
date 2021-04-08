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
		Sensor(char *_name, int pin, int _senseInterval);
		void begin();
		void addToJson(DynamicJsonDocument *doc) const;
		bool checkTemperatureChanged();
		bool checkHumidityChanged();
		float getTemperature();
		float getHumidity();
		char *name;
	private:
		static void taskHandler(void *parameter);
		void taskRunner();
		DHT *dht;
		int senseInterval;
		float temperature;
		float humidity;
		float lastRead_temperature;
		float lastRead_humidity;
};

#endif