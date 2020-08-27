/**
 * 3D-Printer Enclosure Controller firmware for the ESP32
 * Copyright (C) 2020  Marvin Beym
 * 
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 * 
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <https://www.gnu.org/licenses/>.
 **/

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