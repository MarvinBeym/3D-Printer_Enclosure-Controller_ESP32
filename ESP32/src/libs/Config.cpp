#include "Config.h"
#include "ArduinoJson.h"
#include "task-events.h"

Config::Config(EventGroupHandle_t _eg, void (*displayBrightnessEventCallback)(void *))
{
	eg = _eg;

	xTaskCreate(
			displayBrightnessEventCallback,
			"displayBrightnessEventCallback",
			3000,
			nullptr,
			1,
			nullptr
	);
}

void Config::addToJson(DynamicJsonDocument *doc, bool includeDisplayBrightness)
{
	JsonObject jsonObject = doc->createNestedObject("configuration");
	if (includeDisplayBrightness) {
		jsonObject["displayBrightness"] = displayBrightness;
	}
}

void Config::setDisplayBrightness(int brightness)
{
	displayBrightness = brightness;
	xEventGroupSetBits(eg, TASK_EVENT_DisplayBrightness);
}

int Config::getDisplayBrightness()
{
	return displayBrightness;
}
