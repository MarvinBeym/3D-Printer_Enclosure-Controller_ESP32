#pragma once

#include <ArduinoJson.h>

class Config
{
	public:
		Config(EventGroupHandle_t _eg, void (*displayBrightnessEventCallback)(void *));
		void addToJson(DynamicJsonDocument *doc, bool includeDisplayBrightness = true);
		void setDisplayBrightness(int brightness);
		int getDisplayBrightness();
	private:
		int displayBrightness = 100;
		EventGroupHandle_t eg;
};