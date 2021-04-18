#define FASTLED_ALLOW_INTERRUPTS 0
#define FASTLED_INTERNAL

#include <Arduino.h>
#include "FastLED.h"
#include "FasterLed.h"

FasterLed::FasterLed(int _pin, int _numberOfLeds, int _brightness, int _currentLimit)
{
	pin = _pin;
	numberOfLeds = _numberOfLeds;
	brightness = _brightness;
	currentLimit = _currentLimit;
	leds = new CRGB[numberOfLeds];
	pinMode(pin, OUTPUT);
}

void FasterLed::begin()
{
	FastLED.setBrightness(brightness);
	FastLED.setMaxPowerInVoltsAndMilliamps(5, currentLimit);
	clear();
}

void FasterLed::clear()
{
	FastLED.clear(true);
}