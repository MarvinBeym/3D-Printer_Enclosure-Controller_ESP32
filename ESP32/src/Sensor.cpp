#include <Arduino.h>
#include <Adafruit_Sensor.h>
#include <DHT.h>
#include "Sensor.h"

Sensor::Sensor(int pin, int _senseInterval)
{
	senseInterval = _senseInterval;
	previousMillis = 0;
	temperature = 0;
	humidity = 0;
	dht = new DHT(pin, DHT22);
	dht->begin();
}

void Sensor::handle() {
	unsigned long currentMillis = millis();
	if(currentMillis - previousMillis >= senseInterval) {
		previousMillis = currentMillis;
		float tmpHumidity = dht->readHumidity();
		float tmpTemperature = dht->readTemperature();

		if(!isnan(tmpTemperature)){
        	temperature = tmpTemperature;
      	}
      	if (!isnan(tmpHumidity)){
        	humidity = tmpHumidity;
      	}
	}
}