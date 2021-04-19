//Don't change this unless you are modifying the firmware
const String esp32Version = "2.0.0";

const int effects_configSetup_dynamicJsonDocument_size = 5120;
const int led2_dynamicJsonDocument_size = 256 + effects_configSetup_dynamicJsonDocument_size;
const int initial_ws_connect_dynamicJsonDocument_size = 1024 + led2_dynamicJsonDocument_size;


const int dhtSenseInterval = 2000;
const int led2NumberOfLeds = 60;
const int led2CurrentLimit = 1500;
const int led2Brightness = 80;

const int serial1BaudRate = 115200;
const int serial2BaudRate = 115200;

const int displayCompClickCheckInterval = 20;
const int displayBootDelay = 500;
const int displayGraphMinTemp = -10;
const int displayGraphHeight = 120;

//Every x half revolutions the rpm will be calculated so 120 means every 60 full rotations
//Needs to be dividable by 4 as it is also used for a small delay fanSenseInterval / 4 after each calculation.
const int fanSenseInterval = 120;
