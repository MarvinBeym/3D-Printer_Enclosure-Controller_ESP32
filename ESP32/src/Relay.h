#include <Arduino.h>
class Relay{
	protected:
		int pin;
		bool relayState;
		bool offOnHigh;
	public:
		Relay(int pin, bool offOnHigh);
		void begin();
		bool getState();
		void toggle();
		void on();
		void off();
};