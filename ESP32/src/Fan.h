/*
  Class for dealing with 4pin PWM Fans
  Created by Marvin Beym, 02.04.2021.
*/
#ifndef Fan_h
#define Fan_h

#include <Arduino.h>

class Fan
{
	public:
		Fan(int _channel, int tachoPin, int pwmPin);
		int getFanSpeed();
		void incrementHalfRevolution();
		void setDutyCycle(int dutyCycle);
		void setSpeed(int speed);
	private:
		int channel;
		volatile int halfRevolution;
		unsigned long timeOld;
		unsigned int rpm;
		int dutyCycle; 
};

#endif