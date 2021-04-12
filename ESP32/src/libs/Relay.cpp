#include "Relay.h"

/**
 * Relay class for controlling a relay
 * @param _pin The pin where the data is connected to the controller
 * @param _invert Should the digitalWrite be inverted
 */
Relay::Relay(const char *_name, int _pin, EventGroupHandle_t _eg, int _stateUpdateEvent,
			 void (*_stateUpdateCallback)(void *), bool _invert)
{
	name = _name;
	pin = _pin;
	eg = _eg;
	stateUpdateEvent = _stateUpdateEvent;
	stateUpdateCallback = _stateUpdateCallback;
	invert = _invert;

	pinMode(pin, OUTPUT);
	state = false;

	xTaskCreate(
			stateUpdateCallback,
			"relayStateUpdateCallback",
			2000,
			(void *) &state,
			1,
			NULL
	);
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
void Relay::setState(bool _state)
{
	state = _state;
	if (invert) {
		digitalWrite(pin, !_state ? HIGH : LOW);
	} else {
		digitalWrite(pin, _state ? HIGH : LOW);
	}
	xEventGroupSetBits(eg, stateUpdateEvent);
}

/**
 * Sets a new state using an int as the parameter
 * <= 0 = false
 * >= 1 == true
 * @param _state
 */
void Relay::setState(int _state) { setState(_state >= 1); }

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

/**
 * Adds the values of the class/object to the passed json document using the name of the object
 * @param doc
 */
void Relay::addToJson(DynamicJsonDocument *doc) const
{
	JsonObject json = doc->createNestedObject(name);
	json["state"] = state;
}
