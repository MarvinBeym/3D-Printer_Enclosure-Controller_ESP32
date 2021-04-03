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
const int dhtSenseInterval = 2000;
const int led2NumberOfLeds = 60;
const int led2CurrentLimit = 1500;
const int led2Brightness = 80;

const int serial1BaudRate = 115200;
const int serial2BaudRate = 115200;