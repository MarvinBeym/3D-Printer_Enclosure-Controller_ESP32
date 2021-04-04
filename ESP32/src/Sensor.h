/*
  A Class for handling dht22 sensors
  Created by Marvin Beym, 02.04.2021.
*/
#ifndef Sensor_h
#define Sensor_h

#include <Arduino.h>
#include <Adafruit_Sensor.h>
#include <DHT.h>

class Sensor
{
	public:
		Sensor(char *_name, int pin, int _senseInterval);
		void begin();
		float temperature;
		float humidity;
		char *name;
	private:
		static void taskHandler(void *parameter);
		void taskRunner();
		DHT *dht;
		int senseInterval;
};

#endif