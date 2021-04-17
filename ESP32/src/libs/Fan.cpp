#include "Fan.h"
#include "controller-configuration.h"
#include "task-events.h"
#include "helper.h"
/**
 * Fan class constructor
 * @param _name 					The name of the fan, used for ledc setup and json
 * @param _channel 					The ledc channel the fan should run on
 * @param _eg 						The event group handle to check for event changes
 * @param _calcRpmEvent 			The event to wait for before calculating the rpm
 * @param _rpmUpdatedEvent 			The event that gets called when the rpm of the fan changed (gets set by the calcRpmEvent
 * @param tachoPin 					The tacho pin of the fan used (allows reading the rpm)
 * @param pwmPin 					The pwm pin of the fan used (allows changing the speed)
 * @param _rpmUpdatedEventCallback 	The callback that get's called when the rpm of the fan updated
 */
Fan::Fan(const char *_name, int _channel, EventGroupHandle_t _eg, int _calcRpmEvent, int _rpmUpdatedEvent, int _pwmUpdatedEvent, int tachoPin,
		 int pwmPin,
		 void (*_rpmUpdatedEventCallback)(void *),
		 void (*_pwmUpdatedEventCallback)(void *))
{
	pinMode(tachoPin, INPUT);
	pinMode(pwmPin, OUTPUT);
	eg = _eg;
	calcRpmEvent = _calcRpmEvent;
	rpmUpdatedEvent = _rpmUpdatedEvent;
	pwmUpdatedEvent = _pwmUpdatedEvent;

	rpmUpdatedEventCallback = _rpmUpdatedEventCallback;
	pwmUpdatedEventCallback = _pwmUpdatedEventCallback;
	name = _name;
	channel = _channel;

	//Setup ledc (fan pwm control)
	ledcSetup(channel, 25000, 8);
	ledcAttachPin(pwmPin, channel);
	setDutyCycle(0); //Making sure the fan is off first

	//Setting up tasks for rpm calculation & update callback
	xTaskCreate(&taskHandler, name, 1500, this, 1, nullptr);
	xTaskCreate(
			rpmUpdatedEventCallback,
			"rpmUpdateCallback",
			2500,
			(void *) &rpm,
			1,
			nullptr
	);

	xTaskCreate(
			pwmUpdatedEventCallback,
			"pwmUpdatedEventCallback",
			2500,
			nullptr,
			1,
			nullptr
	);
}

/**
 * A wrapper to allow creation of a task inside the class
 * @param parameter
 */
void Fan::taskHandler(void *parameter)
{
	Fan *fan = reinterpret_cast<Fan *>(parameter);
	fan->calculateRpmTask();
}

/**
 * Calculates the rpm when the ISR in the main program sets the event bit
 */
void Fan::calculateRpmTask()
{
	for (;;) {
		xEventGroupWaitBits(eg, calcRpmEvent, pdTRUE, pdTRUE, portMAX_DELAY);
		const long currentMillis = millis();
		const int newRpm = 30 * 1000 / (currentMillis - timeOld) * halfRevolutions;
		timeOld = currentMillis;
		if (newRpm != rpm) {
			rpm = newRpm;
			xEventGroupSetBits(eg, rpmUpdatedEvent);
		}
		halfRevolutions = 0;

		//Small delay to prevent the event from being called again
		delay(fanSenseInterval / 4);
	}
}

/**
 * Allows adding the information about a fan to a json object as a field
 * @param doc
 * @param includeRpm		Should the rpm value be included?
 * @param includePercent	Should the percent value be included?
 * @param includeDutyCycle	Should the duty cycle be included?
 */
void Fan::addToJson(DynamicJsonDocument *doc, bool includeRpm, bool includePercent, bool includeDutyCycle) const
{
	JsonObject json = doc->createNestedObject(name);
	if (includeRpm) {
		json["rpm"] = rpm;
	}
	if (includePercent) {
		json["percent"] = percent;
	}
	if (includeDutyCycle) {
		json["dutyCycle"] = dutyCycle;
	}
}

/**
 * Sets the duty cycle of the fan
 * Making it faster/slower
 * @param _dutyCycle
 */
void Fan::setDutyCycle(int _dutyCycle)
{
	dutyCycle = _dutyCycle;
	percent = Helper::newMap(dutyCycle, 0, 255, 0, 100);

	xEventGroupSetBits(eg, pwmUpdatedEvent);
	ledcWrite(channel, _dutyCycle);
}

/**
 * Sets the fans duty cycle based on a percent value from 0 to 100
 * @param _percent
 */
void Fan::setPercent(int _percent)
{
	percent = _percent;
	setDutyCycle(Helper::newMap(_percent, 0, 100, 0, 255));
}

/**
 * Returns the fans percent value
 * @return
 */
int Fan::getPercent()
{
	return percent;
}

/**
 * Returns the fans duty cycle value
 * @return
 */
int Fan::getDutyCycle()
{
	return dutyCycle;
}