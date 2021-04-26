//
// Created by Marvi on 26.04.2021.
//

#pragma once

class Buzzer
{
	public:
		Buzzer(int _pin, int _channel);
		void setState(bool _state);
		bool getState();
		void on();
		void off();
		void playTone(int percent, int frequency);
		void stopTone();
		bool getPlayingTone();
	private:
		int channel;
		int defaultFrequency = 2000;
		bool state = false;
		bool playingTone = false;

};