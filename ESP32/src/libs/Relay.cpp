#include "Relay.h"

/**
 * Relay class for controlling a relay
 * @param _pin The pin where the data is connected to the controller
 * @param _invert Should the digitalWrite be inverted
 */
Relay::Relay(int _pin, bool _invert)
{
	pin = _pin;
	pinMode(pin, OUTPUT);
	setState(false);
	invert = _invert;
}

/**
 * Returns the current relay state
 * @return
 */
bool Relay::getState() { return state; }

/**
 * Sets a new state
 * @param _state
 */
void Relay::setState(bool _state) {
	if(invert) {
		digitalWrite(pin, !_state ? HIGH : LOW);
	} else {
		digitalWrite(pin, _state ? HIGH : LOW);
	}
}

/**
 * Sets a new state using an int as the parameter
 * <= 0 = false
 * >= 1 == true
 * @param _state
 */
void Relay::setState(int _state) { setState(_state <= 0); }

/**
 * Toggles the relay state
 */
void Relay::toggle() { setState(!state); }

/**
 * Sets the relay state to on
 */
void Relay::on() { setState(true); }

/**
 * Sets the relay state to off
 */
void Relay::off() { setState(false); }