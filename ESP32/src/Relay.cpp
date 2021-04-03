#include "Relay.h"

Relay::Relay(int p, bool _offOnHigh)
{
	pin = p;
	pinMode(pin, OUTPUT);
	relayState = false;
	offOnHigh = _offOnHigh;
}

void Relay::begin()
{

	if (offOnHigh) {
		digitalWrite(pin, HIGH);
	}
	relayState = false;
}

bool Relay::getState()
{
	return relayState;
}

void Relay::toggle()
{
	if (relayState) {
		off();
	} else {
		on();
	}
}

void Relay::on()
{
	if (offOnHigh) {
		digitalWrite(pin, LOW);
	} else {
		digitalWrite(pin, HIGH);
	}
	relayState = true;
}

void Relay::off()
{
	if (offOnHigh) {
		digitalWrite(pin, HIGH);
	} else {
		digitalWrite(pin, LOW);
	}
	relayState = false;
}