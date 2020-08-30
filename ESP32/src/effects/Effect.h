class Effect {
    public:
        const char* name = "";
        virtual void HandleEffect(CRGB* leds, int numberOfLeds, int color, int* additionalValues) = 0;
        void setName(const char* effectName){
            name = effectName;
        }
        const char* getName(){
            return name;
        }
};