/**
 * A Simple class to interact with the nextion display.
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

#include <Arduino.h>

class SimpleNextion{
    private:
        HardwareSerial *nextionHWSerial;
    public:
        SimpleNextion(){};//Empty contructor
        SimpleNextion(HardwareSerial &nextionSerial, uint32_t baudrate);//Constructor
        String ReadCommand();
        void SendCommand(const char* cmdToSend);
        void SetCompValue(String compName, int value);
        void SetCompText(String compName, String value);
        unsigned int GetCompValue(String compName);
        void SetPage(const char* page);
};