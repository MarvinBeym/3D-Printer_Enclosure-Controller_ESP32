#include <Arduino.h>

class Fade: public Effect{
    private:
        unsigned long led2_effect_previousMillis = 0;
        int led2_effect_brightness = 0;
        int led2_effect_fadeAmount = 1;
    public:
        Fade(const char* name){
            setName(name);
        }
        void HandleEffect(CRGB* leds, int numberOfLeds, int color, int* additionalValues){
            unsigned long led2_effect_currentMillis = millis();
            if (led2_effect_currentMillis - led2_effect_previousMillis >= 8) {
                for(int i = 0; i < led2_numberOfLEDs; i++ )
                {
                    leds[i] = color;
                    leds[i].fadeLightBy(triwave8(led2_effect_brightness));
                }
                FastLED.show();
                led2_effect_brightness = led2_effect_brightness + led2_effect_fadeAmount;
                // reverse the direction of the fading at the ends of the fade: 
                if(led2_effect_brightness >= 255)
                {
                    led2_effect_brightness = 0; 
                }
            }
        }
};