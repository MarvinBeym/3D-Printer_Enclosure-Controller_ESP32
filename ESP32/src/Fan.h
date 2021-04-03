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
		Fan(char* _name, int _channel, int tachoPin, int pwmPin);
		void incrementHalfRevolution();
		void setDutyCycle(int dutyCycle);
		void setSpeed(int speed);
		char* name;
		unsigned int rpm;
		unsigned int speed;
		unsigned int dutyCycle; 
	private:
		static void taskHandler(void *parameter);
		void taskRunnner();
		int channel;
		volatile int halfRevolution;
		unsigned long timeOld;
};

#endif