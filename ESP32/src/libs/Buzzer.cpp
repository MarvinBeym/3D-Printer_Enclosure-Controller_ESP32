//
// Created by Marvi on 26.04.2021.
//

#include <Arduino.h>
#include "Buzzer.h"
#include "helper.h"

Buzzer::Buzzer(int _pin, int _channel)
{
	pinMode(_pin, OUTPUT);
	channel = _channel;

	ledcSetup(channel, defaultFrequency, 8);
	ledcAttachPin(_pin, channel);
	ledcWriteTone(channel, defaultFrequency);
	ledcWrite(channel, 0);
}

void Buzzer::setState(bool _state)
{
	if(!playingTone) {
		ledcWrite(channel, _state ? 255 : 0);
		state = _state;
	}
}

bool Buzzer::getState()
{
	return state;
}

void Buzzer::on()
{
	setState(true);
}

void Buzzer::off()
{
	if(!playingTone) {
		setState(false);
	}
}

void Buzzer::playTone(int percent, int frequency = 2000)
{
	if(!state) {
		ledcWrite(channel, Helper::newMap(percent, 0, 100, 0, 255));
		ledcWriteTone(channel, frequency);
		playingTone = true;
	}
}

void Buzzer::stopTone(){
	if(!state) {
		ledcWrite(channel, 0);
		ledcWriteTone(channel, defaultFrequency);
		playingTone = false;
	}
}

bool Buzzer::getPlayingTone()
{
	return playingTone;
}
