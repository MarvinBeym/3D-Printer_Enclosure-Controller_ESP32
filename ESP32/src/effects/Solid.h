#pragma once

class Solid : public Effect
{
	public:
		Solid(const char *_name, bool _effectGetsHandledOnce = false) : Effect{_name, _effectGetsHandledOnce} {};

		bool hasEffectConfig()
		{
			return true;
		}

		void effectHandler(CRGB *leds, int numberOfLeds)
		{
			for (int i = 0; i < numberOfLeds; i++) {
				int colorHtmlNumber = getSelectValue("color");
				leds[i] = colorHtmlNumber;
			}
			FastLED.show();
		}
		void defineEffectConfig() {
			JsonArray colorOptions = addSelect("color", "Color", CRGB::Red);
			colorOptions[0]["red"] = CRGB::Red;
			colorOptions[1]["blue"] = CRGB::Blue;
			colorOptions[2]["yellow"] = CRGB::Yellow;
			colorOptions[3]["orange"] = 0xFF8D33;
			colorOptions[4]["green"] = CRGB::Green;
			colorOptions[5]["pink"] = CRGB::Pink;
			colorOptions[6]["purple"] = CRGB::Purple;
		}
};