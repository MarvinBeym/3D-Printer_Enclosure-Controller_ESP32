#include "Fan.h"

Fan::Fan(char *_name, int _channel, int tachoPin, int pwmPin)
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
	ledcSetup(channel, 25000, 8);
	ledcAttachPin(pwmPin, channel);
}

void Fan::begin()
{
	setDutyCycle(0);
	delay(20);
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
			continue;
		}
		if (halfRevolution < 40) {
			continue;
		}
		rpm = 30 * 1000 / (millis() - timeOld) * halfRevolution;
		timeOld = millis();
		halfRevolution = 0;
	}
}

void Fan::addToJson(DynamicJsonDocument *doc) const
{
	JsonObject json = doc->createNestedObject(name);
	json["percent"] = percent;
	json["rpm"] = rpm;
	json["dutyCycle"] = dutyCycle;
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