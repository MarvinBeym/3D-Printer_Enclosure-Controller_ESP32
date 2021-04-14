class Rainbow : public Effect
{
	public:
		Rainbow(const char *_name, bool _effectGetsHandledOnce = false) : Effect{_name, _effectGetsHandledOnce} {};

		void effectHandler(CRGB *leds, int numberOfLeds)
		{
			uint8_t fillNumber = beat8(100, 300);
			fill_rainbow(leds, numberOfLeds, fillNumber, 255 / numberOfLeds);
			fadeToBlackBy(leds, numberOfLeds, 80);
			FastLED.show();
		}
};