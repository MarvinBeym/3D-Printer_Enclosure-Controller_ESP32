#include "Sensor.h"

Sensor::Sensor(const char *_name, int pin, int _senseInterval, EventGroupHandle_t _eg,
			   int _temperatureUpdateEvent,
			   int _humidityUpdateEvent,
			   int _tempDangerUpdateEvent,
			   void (*temperatureUpdateCallback)(void *),
			   void (*humidityUpdateCallback)(void *),
			   void (*tempDangerUpdateCallback)(void *)
)
{
	name = _name;
	senseInterval = _senseInterval;
	eg = _eg;
	temperatureUpdateEvent = _temperatureUpdateEvent;
	humidityUpdateEvent = _humidityUpdateEvent;
	tempDangerUpdateEvent = _tempDangerUpdateEvent;

	temperature = 0;
	humidity = 0;

	dht = new DHT(pin, DHT22);

	dht->begin();
	delay(100);

	xTaskCreate(&taskHandler, name,
				2000, this, 1, nullptr);

	xTaskCreate(
			temperatureUpdateCallback,
			"temperatureUpdateCallback",
			2000,
			(void *) &temperature,
			1,
			NULL
	);
	xTaskCreate(
			humidityUpdateCallback,
			"humidityUpdateCallback",
			2000,
			(void *) &humidity,
			1,
			NULL
	);

	xTaskCreate(
			tempDangerUpdateCallback,
			"tempDangerUpdateCallback",
			2000,
			nullptr,
			1,
			nullptr
	);
}

//A wrapper static function to allow creation of tasks inside the class
void Sensor::taskHandler(void *parameter)
{
	Sensor *sensor = reinterpret_cast<Sensor *>(parameter);
	sensor->readSensorTask();
}

//Runs the actual task code.
void Sensor::readSensorTask()
{
	for (;;) {
		float tmpHumidity = dht->readHumidity();
		float tmpTemperature = dht->readTemperature();

		if ((!isnan(tmpTemperature) && tmpTemperature != temperature) || temperature == 0) {
			temperature = tmpTemperature;
			xEventGroupSetBits(eg, temperatureUpdateEvent);
		}
		if ((!isnan(tmpHumidity) && tmpHumidity != humidity) || humidity == 0) {
			humidity = tmpHumidity;
			xEventGroupSetBits(eg, humidityUpdateEvent);
		}
		delay(senseInterval);
	}
}

void
Sensor::addToJson(DynamicJsonDocument *doc, bool includeTemperature, bool includeHumidity, bool includeTempWarn) const
{
	JsonObject json = doc->createNestedObject(name);
	if (includeTemperature) {
		json["temperature"] = temperature;
	}
	if (includeHumidity) {
		json["humidity"] = humidity;
	}

	if (includeTempWarn) {
		JsonObject tempDangerObj = json.createNestedObject("tempDanger");

		tempDangerObj["enabled"] = tempDangerEnabled;
		tempDangerObj["threshold"] = tempDangerThreshold;
	}
}

void Sensor::setTempDangerEnabled(bool enabled)
{
	tempDangerEnabled = enabled;
	xEventGroupSetBits(eg, tempDangerUpdateEvent);
}

void Sensor::setTempDangerThreshold(int threshold)
{
	tempDangerThreshold = threshold;
	xEventGroupSetBits(eg, tempDangerUpdateEvent);
}

bool Sensor::getTempDangerEnabled() { return tempDangerEnabled; }

int Sensor::getTempDangerThreshold() { return tempDangerThreshold; }

float Sensor::getTemperature()
{
	return temperature;
}
