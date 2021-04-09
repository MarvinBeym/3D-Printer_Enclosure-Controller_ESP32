#include "Fan.h"
#include "../controller-configuration.h"

Fan::Fan(char *_name, int _channel, int tachoPin, int pwmPin, void (*rpmUpdateCallback)(void *))
{
	pinMode(tachoPin, INPUT);
	pinMode(pwmPin, OUTPUT);

	name = _name;
	channel = _channel;
	halfRevolution = 0;
	timeOld = 0;
	rpm = 0;
	dutyCycle = 0;
	percent = 0;
	this->rpmUpdateCallback = rpmUpdateCallback;
	ledcSetup(channel, 25000, 8);
	ledcAttachPin(pwmPin, channel);
}

void Fan::begin()
{
	setDutyCycle(0);
	xTaskCreate(&Fan::taskHandler, name, 1000, this, 1, nullptr);
}

//A wrapper static function to allow creation of tasks inside the class
void Fan::taskHandler(void *parameter)
{
	Fan *fan = reinterpret_cast<Fan *>(parameter);
	fan->taskRunner();
}

//Runs the actual task code.
void Fan::taskRunner()
{
	for (;;) {
		if (dutyCycle == 0) {
			rpm = 0;
			delay(1000);
		}

		const int newRpm = 30 * 1000 / (millis() - timeOld) * halfRevolution;
		timeOld = millis();
		halfRevolution = 0;
		if (newRpm != rpm) {
			xTaskCreate(
					rpmUpdateCallback,
					"rpmUpdateCallback",
					2000,
					(void *) &rpm,
					1,
					nullptr
			);
		}
		delay(fanSenseInterval);
	}
}

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

void Fan::incrementHalfRevolution()
{
	halfRevolution++;
}

void Fan::setDutyCycle(int _dutyCycle)
{
	dutyCycle = _dutyCycle;
	ledcWrite(channel, _dutyCycle);
}

void Fan::setPercent(int _percent)
{
	percent = _percent;
	setDutyCycle(map(_percent, 0, 100, 0, 255));
}