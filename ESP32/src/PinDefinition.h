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

const int dht22_1_pin = 19;
const int dht22_2_pin = 18;
const int fan1_tacho_pin = 13;
const int fan1_pwm_pin = 14;
const int fan2_tacho_pin = 27;
const int fan2_pwm_pin = 26;
const int buzzer_pin = 4;
const int led1_relay_pin = 21;
const int led2_data_pin = 22;

//EEPROM TABLE
/* ADR | Value saved in ADR
0 | fan1_dutyCycle 
1 | fan2_dutyCycle
2 | led1_enabled
3 | led2_color(byte converted)
4 | led2_mode(byte converted)
5 | display brightness
6 | display sleep
*/