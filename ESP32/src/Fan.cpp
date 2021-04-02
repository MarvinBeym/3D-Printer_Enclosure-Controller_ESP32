#include <Arduino.h>
#include "Fan.h"

Fan::Fan(int _channel, int tachoPin, int pwmPin)
{
	pinMode(tachoPin, INPUT);
	pinMode(pwmPin, OUTPUT);
	channel = _channel;
	halfRevolution = 0;
	timeOld = 0;
	rpm = 0;
	dutyCycle = 0;
	ledcSetup(channel, 25000, 8);
	ledcAttachPin(pwmPin, channel);
	ledcWrite(channel, 0);
}

void Fan::incrementHalfRevolution()
{
	halfRevolution++;
}

int Fan::getFanSpeed()
{
	if(halfRevolution >= 40) {
		rpm = 30*1000/(millis() - timeOld)*halfRevolution;
		timeOld = millis();
		halfRevolution = 0;
	}
	if(dutyCycle == 0) {
		return 0;
	}
	return rpm;
}

void Fan::setDutyCycle(int dutyCycle) {
	ledcWrite(channel, dutyCycle);
}

void Fan::setSpeed(int speed) {
	setDutyCycle(map(speed, 0, 100, 0, 255));
}