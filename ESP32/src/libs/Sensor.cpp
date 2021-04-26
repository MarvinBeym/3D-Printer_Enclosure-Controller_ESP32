#include "Sensor.h"

Sensor::Sensor(const char *_name, int pin, int _senseInterval, EventGroupHandle_t _eg,
			   int _temperatureUpdateEvent,
			   int _humidityUpdateEvent,
			   int _tempWarnUpdateEvent,
			   void (*temperatureUpdateCallback)(void *),
			   void (*humidityUpdateCallback)(void *),
			   void (*tempWarnUpdateCallback)(void *)
)
{
	name = _name;
	senseInterval = _senseInterval;
	eg = _eg;
	temperatureUpdateEvent = _temperatureUpdateEvent;
	humidityUpdateEvent = _humidityUpdateEvent;
	tempWarnUpdateEvent = _tempWarnUpdateEvent;

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
			tempWarnUpdateCallback,
			"tempWarnUpdateCallback",
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

void Sensor::addToJson(DynamicJsonDocument *doc, bool includeTemperature, bool includeHumidity, bool includeTempWarn) const
{
	JsonObject json = doc->createNestedObject(name);
	if (includeTemperature) {
		json["temperature"] = temperature;
	}
	if (includeHumidity) {
		json["humidity"] = humidity;
	}

	if(includeTempWarn) {
		JsonObject tempWarnObj = json.createNestedObject("tempWarning");
		JsonObject warningObj = tempWarnObj.createNestedObject("warning");
		JsonObject dangerObj = tempWarnObj.createNestedObject("danger");

		warningObj["enabled"] = tempWarnEnabled;
		warningObj["threshold"] = tempWarnThreshold;

		dangerObj["enabled"] = tempDangerEnabled;
		dangerObj["threshold"] = tempDangerThreshold;
	}
}