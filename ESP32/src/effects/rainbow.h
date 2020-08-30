class Rainbow: public Effect{
    public:
        Rainbow(const char* name){
            setName(name);
        }
        void HandleEffect(CRGB* leds, int numberOfLeds, int color, int* additionalValues){
            uint8_t fillNumber = beat8(100,300);
            fill_rainbow(leds, led2_numberOfLEDs, fillNumber, 255/led2_numberOfLEDs); 
            fadeToBlackBy(leds, led2_numberOfLEDs, 80);
            FastLED.show();
        }
};