//Don't change this unless you are modifying the firmware
const String esp32Version = "2.0.0";

const int dhtSenseInterval = 2000;
const int led2NumberOfLeds = 60;
const int led2CurrentLimit = 1500;
const int led2Brightness = 80;

const int serial1BaudRate = 115200;
const int serial2BaudRate = 115200;

const int displayBootDelay = 500;
const int displayPageRefreshInterval = 500;
const int displayGraphMinTemp = -10;
const int displayGraphHeight = 120;

const int fanSenseInterval = 60;

//Comment out to disable the feature

//At least one of them has to be enabled or the esp will crash as nobody is feeding the watchdog
#define WEBINTERFACE_ENABLED
#define NEXTION_DISPLAY_ENABLED
