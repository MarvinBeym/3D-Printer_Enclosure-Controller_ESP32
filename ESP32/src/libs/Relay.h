/*
  A Class for handling relays
  Created by Marvin Beym, 02.04.2021.
*/
#include <Arduino.h>
#include <ArduinoJson.h>

class Relay
{
	public:
		Relay(const char *_name, int _pin, EventGroupHandle_t _eg, int _stateUpdateEvent,
			  void (*_stateUpdateCallback)(void *), bool _invert = false);
		bool getState();
		void toggle();
		void on();
		void off();
		void setState(bool state);
		void addToJson(DynamicJsonDocument *doc) const;
		void setState(int state);
	private:
		const char *name;
		void (*stateUpdateCallback)(void *);
		EventGroupHandle_t eg;
		int pin;
		int stateUpdateEvent;
		bool state = false;
		bool invert = false;
};