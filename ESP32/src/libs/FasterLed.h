/*
  A wrapper for FastLED
  Created by Marvin Beym, 02.04.2021.
*/
#pragma once


#include <Arduino.h>
#include <FastLED.h>

class FasterLed
{
	public:
		FasterLed(int _pin, int _numberOfLeds, int _brightness, int _currentLimit);
		void begin();
		int numberOfLeds;
		int brightness;
		int currentLimit;
		CRGB *leds;
		void clear();
	private:
		int pin;
};