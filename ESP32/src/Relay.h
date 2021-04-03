/*
  A Class for handling relays
  Created by Marvin Beym, 02.04.2021.
*/
#include <Arduino.h>

class Relay
{
	public:
		Relay(int pin, bool _offOnHigh);
		void begin();
		bool getState();
		void toggle();
		void on();
		void off();
	private:
		int pin;
		bool relayState;
		bool offOnHigh;
};