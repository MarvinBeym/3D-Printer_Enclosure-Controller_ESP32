#pragma once

class None : public Effect
{
	public:
		None(
				const char *_name,
				bool _effectGetsHandledOnce = true,
				bool _hasEffectConfig = false) :
				Effect{
						_name,
						_effectGetsHandledOnce,
						_hasEffectConfig
				} {};

		void effectHandler(CRGB *leds, int numberOfLeds)
		{
			for (int i = 0; i < numberOfLeds; i++) {
				leds[i] = CRGB::Black;
			}
			FastLED.show();
		}
};