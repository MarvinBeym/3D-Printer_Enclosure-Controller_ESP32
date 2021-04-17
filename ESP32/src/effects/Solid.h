#pragma once

class Solid : public Effect
{
	public:
		Solid(const char *_name, bool _effectGetsHandledOnce = false) : Effect{_name, _effectGetsHandledOnce} {};

		bool getEffectConfigSetupDefined()
		{
			return true;
		}

		void effectHandler(CRGB *leds, int numberOfLeds)
		{
			for (int i = 0; i < numberOfLeds; i++) {
				leds[i] = CRGB::Red;
			}
			FastLED.show();
		}
		void defineEffectConfigSetup(JsonObject *_effectConfigSetupJson) {
			JsonArray jsonColorArray = _effectConfigSetupJson->createNestedArray("colors");
			jsonColorArray[0]["red"] = CRGB::Red;
			jsonColorArray[1]["blue"] = CRGB::Blue;
			jsonColorArray[2]["yellow"] = CRGB::Yellow;
			jsonColorArray[3]["orange"] = 0xFF8D33;
			jsonColorArray[4]["green"] = CRGB::Green;
			jsonColorArray[5]["pink"] = CRGB::Pink;
			jsonColorArray[6]["purple"] = CRGB::Purple;
		}
};