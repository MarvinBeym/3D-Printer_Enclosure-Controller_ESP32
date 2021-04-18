#pragma once
#define FASTLED_INTERNAL

#include <libs/FasterLed.h>
#include "Arduino.h"
#include "ArduinoJson.h"

class Effect
{
	public:
		Effect(const char *_name, bool _effectGetsHandledOnce = false) : name{_name}, effectGetsHandledOnce{
				_effectGetsHandledOnce} {};
		const char *getName();
		void setEffectId(int id);
		bool getEffectHandled();
		void setEffectHandled(bool handled);
		int getEffectId();
		bool getEffectGetsHandledOnce();
		void setupEffectConfig(DynamicJsonDocument *doc);
		virtual void effectHandler(CRGB *leds, int numberOfLeds);
		virtual void defineEffectConfig();
		virtual bool hasEffectConfig();
		JsonArray addSelect(const char *_name, const char *label, int value);
		JsonArray addSelect(const char *_name, const char *label, char *value);
		void addSwitch(const char *_name, const char *label, bool state);
		void changeEffectConfigValue(DynamicJsonDocument document);
	private:
		JsonArray selectsJsonArr;
		JsonArray switchesJsonArr;
		JsonObject effectConfig;
		const char *name;
		bool effectHandled = false;
		bool effectGetsHandledOnce;
		int effectId;
		void updateEffectConfigValue(JsonArray arrayWithNewValues, JsonArray currentArray, const char *keyFieldName,
									 const char *valueFieldName);
	protected:
		bool getSwitchState(const char* _name);
		JsonVariant getSelectValue(const char* _name);
};