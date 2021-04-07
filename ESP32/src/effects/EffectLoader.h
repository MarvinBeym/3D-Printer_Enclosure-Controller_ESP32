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
		EffectLoader(FasterLed *_led2) {
			effects.push_back(new None("none", true));
			effects.push_back(new Solid("solid", true));
			effects.push_back(new Rainbow("rainbow", false));

			led2 = _led2;
			setupEffects();
		}
		void addToJson(DynamicJsonDocument *doc);
	private:
		FasterLed *led2;
		static void taskHandler(void *parameter);
		void taskRunner();
		int currentEffect = NO_EFFECT;
		void setupEffects();
};