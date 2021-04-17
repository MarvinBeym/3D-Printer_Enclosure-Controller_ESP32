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
		Fan(const char *_name, int _channel, EventGroupHandle_t _eg, int _calcRpmEvent, int _rpmUpdatedEvent,
			int _pwmUpdatedEvent, int _tachoPin, int _pwmPin, void (*_rpmUpdatedEventCallback)(void *),
			void (*_pwmUpdatedEventCallback)(void *));
		volatile int halfRevolutions = 0;
		void setDutyCycle(int dutyCycle);
		void setPercent(int percent);
		int getPercent();
		int getDutyCycle();
		void addToJson(DynamicJsonDocument *doc, bool includeRpm = true, bool includePercent = true,
					   bool includeDutyCycle = true) const;
		const char *name;
	private:
		int rpm = 0;
		int percent = 0;
		int dutyCycle = 0;
		unsigned long timeOld = 0;
		int calcRpmEvent;
		int rpmUpdatedEvent;
		int pwmUpdatedEvent;
		EventGroupHandle_t eg;
		void (*rpmUpdatedEventCallback)(void *);
		void (*pwmUpdatedEventCallback)(void *);
		static void taskHandler(void *parameter);
		void calculateRpmTask();
		int channel;

};

#endif