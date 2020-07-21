#include "Relay.h"

Relay::Relay(int p, bool offonHigh){
	pin = p;
	offOnHigh = offonHigh;
}

void Relay::begin(){
	pinMode(pin, OUTPUT);
	if(offOnHigh){
		digitalWrite(pin, HIGH);
	}
	relayState = false;
}

bool Relay::getState(){
	return relayState;
}

void Relay::toggle(){
	if(relayState){
		off();
	}
	else{
		on();
	}
}

void Relay::on(){
	if(offOnHigh){
		digitalWrite(pin, LOW);
	}
	else{
		digitalWrite(pin, HIGH);
	}
	relayState = true;
}
void Relay::off(){
	if(offOnHigh){
		digitalWrite(pin, HIGH);
	}
	else{
		digitalWrite(pin, LOW);
	}
	relayState = false;
}