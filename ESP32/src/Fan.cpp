#include <Arduino.h>
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
	ledcSetup(channel, 25000, 8);
	ledcAttachPin(pwmPin, channel);
	setDutyCycle(0);

	xTaskCreate(&Fan::taskHandler, name, 1000, this, 1, NULL);
}

//A wrapper static function to allow creation of tasks inside the class
void Fan::taskHandler(void *parameter)
{
	Fan *fan = reinterpret_cast<Fan *>(parameter);
	fan->taskRunnner();
}

//Runs the actual task code.
void Fan::taskRunnner()
{
	for (;;)
	{
		if (dutyCycle == 0)
		{
			rpm = 0;
			continue;
		}
		if (halfRevolution < 40)
		{
			continue;
		}
		rpm = 30 * 1000 / (millis() - timeOld) * halfRevolution;
		timeOld = millis();
		halfRevolution = 0;
	}
}

void Fan::incrementHalfRevolution()
{
	halfRevolution++;
}

void Fan::setDutyCycle(int dutyCycle)
{
	this->dutyCycle = dutyCycle;
	ledcWrite(channel, dutyCycle);
}

void Fan::setSpeed(int speed)
{
	this->speed = speed;
	setDutyCycle(map(speed, 0, 100, 0, 255));
}