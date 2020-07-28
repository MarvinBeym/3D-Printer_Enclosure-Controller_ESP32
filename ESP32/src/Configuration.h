const int fan1_max_rpm = 3000;
const int fan2_max_rpm = 1800;
const int led2_numberOfLEDs = 60;
const int led2_currentLimit = 1500;
const int dht_1_min_tempGraph = -20;
const int dht_2_min_tempGraph = -20;
const long dht_sense_interval = 2000;
const long display_update_interval = 1500;
const long temperature_warner_checkInterval = 2000;
uint8_t wifi_timeout = 8;
const bool rebootOnHotspot = true;
//Wifi
const char* wifi_ssid = "enclC_ESP32";
const char* wifi_password =  "enclC_ESP32";

const char* wifi_ap_ssid = "enclC_ESP32";
const char* wifi_ap_password = "enclC_ESP32";

const char* wifi_hostname = "enclC_ESP32";