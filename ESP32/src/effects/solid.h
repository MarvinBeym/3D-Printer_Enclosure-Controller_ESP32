#include <Arduino.h>

class Solid: public Effect{
    public:
        Solid(const char* name){
            setName(name);
        }
        void HandleEffect(CRGB* leds, int numberOfLeds, int color, int* additionalValues){
            for(int i = 0; i < led2_numberOfLEDs; i++){
                leds[i] = color;
            }
            FastLED.show();
        }
};