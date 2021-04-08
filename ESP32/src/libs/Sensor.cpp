#include "Sensor.h"

Sensor::Sensor(char *_name, int pin, int _senseInterval)
{
	name = _name;
	senseInterval = _senseInterval;
	temperature = 0;
	humidity = 0;
	dht = new DHT(pin, DHT22);
}

void Sensor::begin()
{
	dht->begin();
	delay(100); //Required to wait a bit. This prevents the first Sensor object from being able to read the dht22
	xTaskCreate(&Sensor::taskHandler, name, 1000, this, 3, nullptr);
}

//A wrapper static function to allow creation of tasks inside the class
void Sensor::taskHandler(void *parameter)
{
	Sensor *sensor = reinterpret_cast<Sensor *>(parameter);
	sensor->taskRunner();
}

void Sensor::addToJson(DynamicJsonDocument *doc) const {
	JsonObject json = doc->createNestedObject(name);
	json["humidity"] = humidity;
	json["temperature"] = temperature;
}

//Runs the actual task code.
void Sensor::taskRunner()
{
	for (;;) {
		float tmpHumidity = dht->readHumidity();
		float tmpTemperature = dht->readTemperature();

		if (!isnan(tmpTemperature)) {
			temperature = tmpTemperature;
		}
		if (!isnan(tmpHumidity)) {
			humidity = tmpHumidity;
		}
		vTaskDelay(pdMS_TO_TICKS(senseInterval));
	}
}

bool Sensor::checkTemperatureChanged()
{
	return temperature != lastRead_temperature;
}

bool Sensor::checkHumidityChanged()
{
	return temperature != lastRead_temperature;
}

float Sensor::getTemperature()
{
	lastRead_temperature = temperature;
	return temperature;
}

float Sensor::getHumidity()
{
	lastRead_temperature = temperature;
	return humidity;
}
