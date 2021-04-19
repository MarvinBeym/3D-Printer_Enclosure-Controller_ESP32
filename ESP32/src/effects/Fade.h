#pragma once

class Fade : public Effect
{
	public:
		Fade(
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
				leds[i] = (int) getSelectFieldValue("color");
				leds[i].fadeLightBy(triwave8(brightness));
			}
			FastLED.show();
			brightness += (int) getNumberFieldValue("fadeAmount");
			if (brightness >= 255) {
				brightness = 0;
			}
			delay(8);
		}

		void defineEffectConfig()
		{
			JsonArray colorOptions = addSelectField("color", "Color", CRGB::Red);
			colorOptions[0]["red"] = CRGB::Red;
			colorOptions[1]["white"] = CRGB::White;
			colorOptions[2]["blue"] = CRGB::Blue;
			colorOptions[3]["yellow"] = CRGB::Yellow;
			colorOptions[4]["orange"] = CRGB::OrangeRed;
			colorOptions[5]["green"] = CRGB::Green;
			colorOptions[6]["pink"] = CRGB::DeepPink;
			colorOptions[7]["purple"] = CRGB::Purple;
			addNumberField("fadeAmount", "Fade amount", 1, 0, 120);
		}

	private:
		int brightness = 0;
};