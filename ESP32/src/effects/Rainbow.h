#pragma once

class Rainbow : public Effect
{
	public:
		Rainbow(
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
			uint8_t fillNumber = beat8(
					getNumberFieldValue("beatsPerMinute"),
					300);
			fill_rainbow(leds, numberOfLeds, fillNumber, 255 / numberOfLeds);
			FastLED.show();
		}

		void defineEffectConfig()
		{
			addNumberField("beatsPerMinute", "Beats per minute", 100, 0, 250);
		}
};