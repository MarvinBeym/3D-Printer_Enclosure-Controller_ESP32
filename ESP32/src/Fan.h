/*
  Class for dealing with 4pin PWM Fans
  Created by Marvin Beym, 02.04.2021.
*/
#ifndef Fan_h
#define Fan_h

#include <Arduino.h>
#include <ArduinoJson.h>

class Fan
{
	public:
		Fan(char *_name, int _channel, int tachoPin, int pwmPin);
		void begin();
		void incrementHalfRevolution();
		void setDutyCycle(int _dutyCycle);
		void setPercent(int _percent);
		void addToJson(DynamicJsonDocument *doc) const;
		char *name;
		int rpm;
		int percent;
		unsigned int dutyCycle;
	private:
		static void taskHandler(void *parameter);
		void taskRunner();
		int channel;
		volatile int halfRevolution;
		unsigned long timeOld;
};

#endif