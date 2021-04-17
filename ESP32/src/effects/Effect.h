#pragma once

#include <libs/FasterLed.h>
#include "Arduino.h"

class Effect
{
	public:
		Effect(const char *_name, bool _effectGetsHandledOnce = false) : name{_name}, effectGetsHandledOnce{_effectGetsHandledOnce} {};
		const char *getName();
		void setEffectId(int id);
		bool getEffectHandled();
		void setEffectHandled(bool handled);
		int getEffectId();
		bool getEffectGetsHandledOnce();
		virtual void effectHandler(CRGB *leds, int numberOfLeds);
	private:
		const char *name;
		bool effectHandled = false;
		bool effectGetsHandledOnce;
		int effectId;
};