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
		Fan(char *name, int channel, EventGroupHandle_t eg, int calcRpmEvent, int rpmUpdatedEvent, int tachoPin, int pwmPin, void (*rpmUpdatedEventCallback)(void *));
		volatile int halfRevolutions;
		void setDutyCycle(int dutyCycle);
		void setPercent(int percent);
		int getPercent();
		int getDutyCycle();
		void addToJson(DynamicJsonDocument *doc, bool includeRpm = true, bool includePercent = true, bool includeDutyCycle = true) const;
		char *name;
	private:
		int rpm;
		int percent;
		unsigned int dutyCycle;
		int calcRpmEvent;
		int rpmUpdatedEvent;
		EventGroupHandle_t eg;
		void (*rpmUpdatedEventCallback)(void *);
		static void taskHandler(void *parameter);
		void calculateRpmTask();
		int channel;
		unsigned long timeOld;
};

#endif