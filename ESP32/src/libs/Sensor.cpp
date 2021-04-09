#include "Sensor.h"

Sensor::Sensor(char *_name, int pin, int _senseInterval, void (*temperatureUpdateCallback)(void *),
			   void (*humidityUpdateCallback)(void *))
{
	name = _name;
	senseInterval = _senseInterval;
	temperature = 0;
	humidity = 0;
	dht = new DHT(pin, DHT22);
	this->temperatureUpdateCallback = temperatureUpdateCallback;
	this->humidityUpdateCallback = humidityUpdateCallback;
}

void Sensor::begin()
{
	dht->begin();
	delay(100); //Required to wait a bit. This prevents the first Sensor object from being able to read the dht22
	xTaskCreate(&taskHandler, name, 1000, this, 3, nullptr);
}

//A wrapper static function to allow creation of tasks inside the class
void Sensor::taskHandler(void *parameter)
{
	Sensor *sensor = reinterpret_cast<Sensor *>(parameter);
	sensor->taskRunner();
}

void Sensor::addToJson(DynamicJsonDocument *doc, bool includeTemperature, bool includeHumidity) const
{
	JsonObject json = doc->createNestedObject(name);
	if (includeTemperature) {
		json["temperature"] = temperature;
	}
	if (includeHumidity) {
		json["humidity"] = humidity;
	}
}

//Runs the actual task code.
void Sensor::taskRunner()
{
	for (;;) {
		float tmpHumidity = dht->readHumidity();
		float tmpTemperature = dht->readTemperature();

		if ((!isnan(tmpTemperature) && tmpTemperature != temperature) || temperature == 0) {
			temperature = tmpTemperature;
			xTaskCreate(
					temperatureUpdateCallback,
					"temperatureUpdateCallback",
					2000,
					(void *) &temperature,
					1,
					NULL
			);
		}
		if ((!isnan(tmpHumidity) && tmpHumidity != humidity) || humidity == 0) {
			humidity = tmpHumidity;
			xTaskCreate(
					humidityUpdateCallback,
					"humidityUpdateCallback",
					2000,
					(void *) &humidity,
					1,
					NULL
			);
		}
		delay(senseInterval);
	}
}