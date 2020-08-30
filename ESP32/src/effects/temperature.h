#include <Arduino.h>
/*
additionalValues[0] = dht_1_temperature
additionalValues[1] = dht_2_temperature
additionalValues[2] = temperature_warnThreshold
additionalValues[3] = temperature_dangThreshold


*/
class Temperature: public Effect{
    public:
        Temperature(const char* name){
            setName(name);
        }
        void fill_solid_fromTo(struct CRGB * leds, int fromToFill, int toToFill, const struct CRGB& color){
            for( int i = fromToFill; i < toToFill; i++) {
                leds[i] = color;
            }
        }
        void HandleEffect(CRGB* leds, int numberOfLeds, int color, int* additionalValues){
            if(additionalValues[0] <= 0){fill_solid_fromTo(leds, 0, 29, CRGB::SkyBlue);}
            else if(additionalValues[0] < additionalValues[2]){fill_solid_fromTo(leds, 0, 29, CRGB::Green);}
            else if(additionalValues[0] < additionalValues[3]){fill_solid_fromTo(leds, 0, 29, CRGB::OrangeRed);}
            else if(additionalValues[0] > additionalValues[3]){fill_solid_fromTo(leds, 0, 29, CRGB::Red);}

            if(additionalValues[1] <= 0){fill_solid_fromTo(leds, 30, 60, CRGB::SkyBlue);}
            else if(additionalValues[1] < additionalValues[2]){fill_solid_fromTo(leds, 30, 60, CRGB::Green);}
            else if(additionalValues[1] < additionalValues[3]){fill_solid_fromTo(leds, 30, 60, CRGB::OrangeRed);}
            else if(additionalValues[1] > additionalValues[3]){fill_solid_fromTo(leds, 30, 60, CRGB::Red);}
            FastLED.show();
        }
};