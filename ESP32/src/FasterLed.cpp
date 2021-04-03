#define FASTLED_ALLOW_INTERRUPTS 0

#include <Arduino.h>
#include "FastLED.h"
#include "FasterLed.h"

FasterLed::FasterLed(int pin, int _numberOfLeds, int _brightness, int _currentLimit)
{
	numberOfLeds = _numberOfLeds;
	brightness = _brightness;
	currentLimit = _currentLimit;
	leds = new CRGB[numberOfLeds];

	pinMode(pin, OUTPUT);

	FastLED.setBrightness(brightness);
	FastLED.setMaxPowerInVoltsAndMilliamps(5, currentLimit);
}

void FasterLed::clear()
{
	FastLED.clear(true);
}