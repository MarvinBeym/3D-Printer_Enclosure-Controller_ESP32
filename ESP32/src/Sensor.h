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
		Sensor(int pin, int _senseInterval);
		void handle();
		float temperature;
		float humidity;
	private:
		DHT *dht;
		int senseInterval;
		int previousMillis;
		
};

#endif