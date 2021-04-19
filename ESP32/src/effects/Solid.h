#pragma once

class Solid : public Effect
{
	public:
		Solid(
				const char *_name,
				bool _effectGetsHandledOnce = true,
				bool _hasEffectConfig = false) : Effect{_name,
												_effectGetsHandledOnce,
												_hasEffectConfig} {};

		void effectHandler(CRGB *leds, int numberOfLeds)
		{
			for (int i = 0; i < numberOfLeds; i++) {
				int colorHtmlNumber = getSelectFieldValue("color");
				leds[i] = colorHtmlNumber;
			}
			FastLED.show();
		}
		void defineEffectConfig() {
			JsonArray colorOptions = addSelectField("color", "Color", CRGB::Red);
			colorOptions[0]["red"] = CRGB::Red;
			colorOptions[1]["white"] = CRGB::White;
			colorOptions[2]["blue"] = CRGB::Blue;
			colorOptions[3]["yellow"] = CRGB::Yellow;
			colorOptions[4]["orange"] = CRGB::OrangeRed;
			colorOptions[5]["green"] = CRGB::Green;
			colorOptions[6]["pink"] = CRGB::DeepPink;
			colorOptions[7]["purple"] = CRGB::Purple;
		}
};