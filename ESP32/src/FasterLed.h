/*
  A wrapper for FastLED
  Created by Marvin Beym, 02.04.2021.
*/
#ifndef FasterLed_h
#define FasterLed_h

#include <Arduino.h>
#include <FastLED.h>

class FasterLed
{
	public:
		FasterLed(int pin, int _numberOfLeds, int _brightness, int _currentLimit);
		int numberOfLeds;
		int brightness;
		int currentLimit;
		CRGB *leds;
		void clear();
	private:
};

#endif