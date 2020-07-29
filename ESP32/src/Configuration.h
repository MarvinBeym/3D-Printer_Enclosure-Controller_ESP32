const int fan1_max_rpm = 3000; //Prevents the fan1 tacho rpm from overshooting.
const int fan2_max_rpm = 1800; //Prevents the fan1 tacho rpm from overshooting.
const int led2_numberOfLEDs = 60; //The number of single addressable leds connected to the led2 port.
const int led2_currentLimit = 1500; //current limit. Will prevent the leds from using to much current.
									// In theory this can be max 5A (when using XL4015 buck converter -> take into account display, esp and other stuff).
const int dht_1_minTemp = -20; //the minimum temperature used both for the nextion graph and other stuff
const int dht_2_minTemp = -20; //the minimum temperature used both for the nextion graph and other stuff
const long dht_sense_interval = 2000; //how oftern to update the stored temperature (for DHT22 the recommended time is 2seconds => 2000ms).
const long display_update_interval = 1500; //how often to update the values displayed on the display (to fast and it will cause errors).
const long temperature_warner_checkInterval = 2000; //how often to check for temperature threshold exceeding.

//Wifi
const char* wifi_ap_ssid = "enclC_ESP32"; //WiFi SSID when in AP mode.
const char* wifi_ap_password = "enclC_ESP32"; //WiFi Password when in AP mode.
const char* wifi_hostname = "enclC_ESP32"; //WiFi hostname both for AP and Client.

const bool wifiManagerDarkMode = true; //Should the webinterface from WifiManager be in dark mode
const bool wifiManagerBlocking = true; //Should WifiManager prevent the esp32 from booting fully until a network has been set 
									   //(if false will fully boot and wait for connection in the background)

const int wifiManagerTimeout = 60; //If wifiManagerBlocking = true, this will set the timeout (in seconds) to wait before canceling wifiManager setup and fully booting the esp32
const bool rebootOnHotspot = false; //Should the ESP32 reboot when going into hotspot (incase you are sure everything is correct with your setup and you want to try again).
const bool wifiManagerDebug = true; //WifiManagers debug function.
const int wifiPassSecret = true; //Should the password be replaced by '********' on the nextion display.