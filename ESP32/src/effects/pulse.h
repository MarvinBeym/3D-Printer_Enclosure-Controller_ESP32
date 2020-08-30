#include <Arduino.h>

class Pulse: public Effect{
    private:
        unsigned long led2_effect_previousMillis = 0;
        int led2_effect_counter1 = 0;
        int led2_effect_counter2 = -1;
    public:
        Pulse(const char* name){
            setName(name);
        }
        void HandleEffect(CRGB* leds, int numberOfLeds, int color, int* additionalValues){
            if(led2_effect_counter2 == -1){
                led2_effect_counter2 = numberOfLeds;
            }
            unsigned long led2_effect_currentMillis = millis();
            if (led2_effect_currentMillis - led2_effect_previousMillis >= 50) {
                led2_effect_previousMillis = led2_effect_currentMillis;
            if(led2_effect_counter1 <= led2_numberOfLEDs){
                leds[led2_effect_counter1] = color;
                led2_effect_counter1++;
            }
            if(led2_effect_counter1 >= led2_numberOfLEDs){
                leds[led2_effect_counter2] = CRGB::Black;
                led2_effect_counter2--;
            }

            if(led2_effect_counter1 > led2_numberOfLEDs && led2_effect_counter2 < 0){
                led2_effect_counter1 = 0;
                led2_effect_counter2 = led2_numberOfLEDs;
            }
            FastLED.show();
            }
        }
};