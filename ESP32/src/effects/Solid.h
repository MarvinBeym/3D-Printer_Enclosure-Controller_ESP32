class Solid : public Effect
{
	public:
		Solid(String _name, bool _effectGetsHandledOnce = false) : Effect{_name, _effectGetsHandledOnce} {};
		void effectHandler(CRGB *leds, int numberOfLeds) {
			for(int i = 0; i < numberOfLeds; i++){
				leds[i] = CRGB::Red;
			}
			FastLED.show();
		}
};