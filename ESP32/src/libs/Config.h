#pragma once

#include <ArduinoJson.h>

class Config
{
	public:
		Config(EventGroupHandle_t _eg, void (*configUpdateEventCallback)(void *));
		void addToJson(DynamicJsonDocument *doc, bool includeDisplayBrightness = true, bool includeDisplaySleep = true);
		void setDisplayBrightness(int brightness);
		int getDisplayBrightness();
		void setDisplaySleep(bool sleep);
		bool getDisplaySleep();
	private:
		int displayBrightness = 100;
		bool displaySleep = false;
		EventGroupHandle_t eg;
};