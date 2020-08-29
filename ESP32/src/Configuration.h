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
const int led2_numberOfLEDs = 60; //The number of single addressable leds connected to the led2 port.
const int led2_currentLimit = 1500; //current limit. Will prevent the leds from using to much current.
									// In theory this can be max 5A (when using XL4015 buck converter -> take into account display, esp and other stuff).
const int dht_1_minTemp = -20; //the minimum temperature used both for the nextion graph and other stuff
const int dht_2_minTemp = -20; //the minimum temperature used both for the nextion graph and other stuff
const long dht_sense_interval = 2000; //how often to update the stored temperature (for DHT22 the recommended time is 2seconds => 2000ms).
const long display_update_interval = 1500; //how often to update the values displayed on the display (to fast and it will cause errors).
const long temperature_warner_checkInterval = 2000; //how often to check for temperature threshold exceeding.

//Wifi
const char* wifi_ap_ssid = "enclC_ESP32"; //WiFi SSID when in AP mode.
const char* wifi_ap_password = "enclC_ESP32"; //WiFi Password when in AP mode.
const char* wifi_hostname = "enclC_ESP32"; //WiFi hostname both for AP and Client.

const bool wifiManagerDarkMode = true; 	//Should the webinterface from WifiManager be in dark mode
const bool wifiManagerBlocking = true; 	//Should WifiManager prevent the esp32 from booting fully until a network has been set 
									   	//(if false will fully boot and wait for connection in the background)
										//This will also render the "Confirm" and "Reboot" buttons on the display useless as no communication can happen.

const int wifiManagerTimeout = 60; //If wifiManagerBlocking = true, this will set the timeout (in seconds) to wait before canceling wifiManager setup and fully booting the esp32
const bool rebootOnHotspot = false; //Should the ESP32 reboot when going into hotspot mode (incase you are sure everything is correct with your setup and you want to try again).
const bool wifiManagerDebug = false; //WifiManagers debug function.
const int wifiPassSecret = true; //Should the password be replaced by '********' on the nextion display.