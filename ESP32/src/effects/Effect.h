#include <libs/FasterLed.h>
#include "Arduino.h"

class Effect
{
	public:
		Effect(String _name, bool _effectGetsHandledOnce = false) : name{_name}, effectGetsHandledOnce{_effectGetsHandledOnce} {};
		String getName();
		void setEffectId(int id);
		bool getEffectHandled();
		void setEffectHandled(bool handled);
		int getEffectId();
		bool getEffectGetsHandledOnce();
		virtual void effectHandler(CRGB *leds, int numberOfLeds);
	private:
		String name;
		bool effectHandled = false;
		bool effectGetsHandledOnce;
		int effectId;
};