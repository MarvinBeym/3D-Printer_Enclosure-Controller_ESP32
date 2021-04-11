#include "Effect.h"
#include <Arduino.h>
#include <libs/FasterLed.h>
#include "Solid.h"
#include <ArduinoJson.h>
#include "None.h"
#include "Rainbow.h"
#define NO_EFFECT 0

class EffectLoader{
	public:
		std::vector<Effect*> effects;
		void changeEffect(int newEffectId);
		int getCurrentEffect();
		void addToJson(DynamicJsonDocument *doc);
		EffectLoader(FasterLed *_led2, EventGroupHandle_t _eg, int _effectChangeEvent, void (*_effectChangeCallback)(void *)) {
			effects.push_back(new None("none", true));
			effects.push_back(new Solid("solid", true));
			effects.push_back(new Rainbow("rainbow", false));


			led2 = _led2;
			eg = _eg;
			effectChangeEvent = _effectChangeEvent;
			effectChangeCallback = _effectChangeCallback;
			setupEffects();
		}
	private:
		FasterLed *led2;
		EventGroupHandle_t eg;
		int effectChangeEvent;
		void (*effectChangeCallback)(void *);
		static void taskHandler(void *parameter);
		void taskRunner();
		int currentEffect = NO_EFFECT;
		void setupEffects();
};