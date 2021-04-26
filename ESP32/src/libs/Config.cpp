#include "Config.h"
#include "ArduinoJson.h"
#include "task-events.h"

Config::Config(EventGroupHandle_t _eg, void (*configUpdateEventCallback)(void *))
{
	eg = _eg;

	xTaskCreate(
			configUpdateEventCallback,
			"configUpdateEventCallback",
			3000,
			nullptr,
			1,
			nullptr
	);
}

void Config::addToJson(DynamicJsonDocument *doc, bool includeDisplayBrightness, bool includeDisplaySleep)
{
	JsonObject jsonObject = doc->createNestedObject("configuration");
	if (includeDisplayBrightness) {
		jsonObject["displayBrightness"] = displayBrightness;
	}
	if(includeDisplaySleep) {
		jsonObject["displaySleep"] = displaySleep;
	}
}

void Config::setDisplayBrightness(int brightness)
{
	displayBrightness = brightness;
	xEventGroupSetBits(eg, TASK_EVENT_ConfigUpdate);
}

void Config::setDisplaySleep(bool sleep)
{
	displaySleep = sleep;
	xEventGroupSetBits(eg, TASK_EVENT_ConfigUpdate);
}

int Config::getDisplayBrightness()
{
	return displayBrightness;
}

bool Config::getDisplaySleep()
{
	return displaySleep;
}