#pragma once

class Running : public Effect
{
	public:
		Running(
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
			const int color = getSelectFieldValue("color");
			const int backgroundColor = getSelectFieldValue("backgroundColor");
			//downCounter initialization
			if(downCounter == -1) {
				downCounter = numberOfLeds;
			}

			//Up animation
			if(upCounter <= numberOfLeds) {
				leds[upCounter] = color;
				if(upCounter > 0) {
					leds[upCounter - 1] = backgroundColor;
				}
				upCounter++;
			}

			//Down animation
			if(upCounter >= numberOfLeds) {
				leds[downCounter] = color;
				if(downCounter < numberOfLeds) {
					leds[downCounter + 1] = backgroundColor;
				}
				downCounter--;
			}

			//Resetting after first up / down effect
			if(upCounter > numberOfLeds && downCounter < 0) {
				upCounter = 0;
				downCounter = numberOfLeds;
			}

			FastLED.show();
			delay(getNumberFieldValue("delay"));
		}

		void defineEffectConfig()
		{
			JsonArray colorOptions = addSelectField("color", "Running color", CRGB::Red);
			colorOptions[0]["black"] = CRGB::Black;
			colorOptions[1]["red"] = CRGB::Red;
			colorOptions[2]["white"] = CRGB::White;
			colorOptions[3]["blue"] = CRGB::Blue;
			colorOptions[4]["yellow"] = CRGB::Yellow;
			colorOptions[5]["orange"] = CRGB::OrangeRed;
			colorOptions[6]["green"] = CRGB::Green;
			colorOptions[7]["pink"] = CRGB::DeepPink;
			colorOptions[8]["purple"] = CRGB::Purple;

			JsonArray backgroundOptions = addSelectField("backgroundColor", "Background color", CRGB::Black);
			backgroundOptions.set(colorOptions);
			addNumberField("delay", "Delay", 50, 1);
		}
	private:
		int upCounter = 0;
		int downCounter = -1;
};