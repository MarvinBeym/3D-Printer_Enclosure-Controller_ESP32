/*
  A Class for handling relays
  Created by Marvin Beym, 02.04.2021.
*/
#include <Arduino.h>

class Relay
{
	public:
		Relay(int _pin, bool _invert = false);
		bool getState();
		void toggle();
		void on();
		void off();
		void setState(bool state);
		void setState(int state);
	private:
		int pin;
		bool state = false;
		bool invert = false;
};