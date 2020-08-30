#include "effects/Effect.h"

#include "effects/solid.h"
#include "effects/fade.h"
#include "effects/running.h"
#include "effects/pulse.h"
#include "effects/rainbow.h"
#include "effects/temperature.h"

#include <Arduino.h>

class EffectsLoader{
    private:
        Effect *effectObjects[6];
        CRGB* leds;
        int numberOfLeds;
    public:
        EffectsLoader(){};//Empty contructor
        EffectsLoader(CRGB* ledsArr, int nLeds){
            leds = ledsArr;
            numberOfLeds = nLeds;
            effectObjects[0] = new Solid("SOLID");
            effectObjects[1] = new Fade("FADE");
            effectObjects[2] = new Running("RUNNING");
            effectObjects[3] = new Pulse("PULSE");
            effectObjects[4] = new Rainbow("RAINBOW");
            effectObjects[5] = new Temperature("TEMPERATURE");
        };
        int getNumberOfEffects(){
            return sizeof(effectObjects) / 4;
        }
        const char* getNameOfEffect(int index){
            return effectObjects[index]->getName();
        }
        void HandleEffects(int effect, int color, int* additionalValues){
            effectObjects[effect]->HandleEffect(leds, numberOfLeds, color, additionalValues);
        };
};