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
		virtual bool hasEffectConfig();
		void changeEffectFieldValue(DynamicJsonDocument document);
	private:
		JsonArray selectFields;
		JsonArray switchFields;
		JsonArray numberFields;
		JsonObject effectConfig;
		const char *name;
		bool effectHandled = false;
		bool effectGetsHandledOnce;
		int effectId;
		virtual void defineEffectConfig();
		void updateEffectFieldValue(JsonArray arrayWithNewValues, JsonArray currentArray);
		JsonVariant getConfigFieldValue(const char *_name, JsonArray jsonArr);
	protected:
		bool getSwitchFieldState(const char *_name);
		JsonVariant getSelectFieldValue(const char *_name);
		JsonVariant getNumberFieldValue(const char *_name);
		JsonArray addSelectField(const char *_name, const char *label, int value);
		JsonArray addSelectField(const char *_name, const char *label, char *value);
		void addNumberField(const char *_name, const char *label, int value, int min, int max);
		void addSwitchField(const char *_name, const char *label, bool state);
};