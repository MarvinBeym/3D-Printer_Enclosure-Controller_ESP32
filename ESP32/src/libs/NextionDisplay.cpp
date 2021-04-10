#include <Arduino.h>
#include "NextionDisplay.h"
#include "../controller-configuration.h"

//Extracted from https://nextion.tech/instruction-set/
const int NEXTION_STARTUP = 0x00;
const int SERIAL_BUFFER_OVERFLOW = 0x24;
const int TOUCH_EVENT = 0x65;
const int CURRENT_PAGE_NUMBER = 0x66;
const int TOUCH_COORDINATE_AWAKE = 0x67;
const int TOUCH_COORDINATE_SLEEP = 0x68;
const int STRING_DATA = 0x70;
const int NUMBER_DATA = 0x71;
const int AUTO_ENTERED_SLEEP = 0x86;
const int AUTO_WAKE_FROM_SLEEP = 0x87;
const int NEXTION_READY = 0x88;

/**
 * Constructor of the nextion display class
 * @param bootDelay
 * @param baudRate
 * @param rxPin
 * @param txPin
 * @param _eg
 * @param _compClickedEvent
 * @param _compClickedCallback
 */
NextionDisplay::NextionDisplay(int bootDelay, int baudRate, int rxPin, int txPin, EventGroupHandle_t _eg, int _compClickedEvent, void (*_compClickedCallback)(void *))
{
	currentPage = BOOT_PAGE;
	eg = _eg;
	compClickedEvent = _compClickedEvent;
	compClickedCallback = _compClickedCallback;

	Serial2.begin(baudRate, SERIAL_8N1, rxPin, txPin);
	while (!Serial2) {}
	xTaskCreate(&taskHandler, "nextionDisplay",
				1000, this, 1, nullptr);

	xTaskCreate(
			compClickedCallback,
			"compClickedCallback",
			2000,
			NULL,
			1,
			NULL
	);

	setPage(BOOT_PAGE);
	delay(bootDelay);
	setPage(MAIN_PAGE);
}

/**
 * Changes the page by sending the command over Serial2
 * @param page
 */
void NextionDisplay::setPage(int page)
{
	currentPage = page;
	String cmd = "page " + String(page);
	this->sendCommand(cmd.c_str());
}

/**
 * Sends a command to the nextion over Serial2
 * Will append 3x 0xff
 * @param cmdToSend
 */
void NextionDisplay::sendCommand(const char *cmdToSend)
{
	while (Serial2.available()) {
		Serial2.read();
	}

	Serial2.print(cmdToSend);
	Serial2.write(0xff);
	Serial2.write(0xff);
	Serial2.write(0xff);
}

/**
 * Static function to allow creating of a task in the class/object
 * @param parameter
 */
void NextionDisplay::taskHandler(void *parameter)
{
	auto *nextion = reinterpret_cast<NextionDisplay *>(parameter);
	nextion->checkComponentClicked();
}

/**
 * Checks if a component was clicked (the component needs to send a touch event setup with the nextion software
 */
void NextionDisplay::checkComponentClicked()
{
	for (;;) {
		if(getComponentClicked(compClicked_pageId, compClicked_compId)) {
			xEventGroupSetBits(eg, compClickedEvent);
		}
		delay(displayCompClickCheckInterval);
	}
}

/**
 * Sets a components value (integer)
 * @param compVarString
 * @param value
 * @return
 */
NextionDisplay *NextionDisplay::setCompValue(String compVarString, int value)
{
	String newValue = compVarString + ".val=" + value;
	sendCommand(newValue.c_str());
	return this;
}

/**
 * Gets the value of a component
 * @param compVarString
 * @return
 */
int NextionDisplay::getCompValue(String compVarString)
{
	int value = 0;
	String cmd = "get " + compVarString + ".val";
	sendCommand(cmd.c_str());
	Serial2.setTimeout(100);
	uint8_t receivedBytes[8] = {0};
	if(sizeof(receivedBytes) != Serial2.readBytes((char *)receivedBytes, sizeof(receivedBytes))){
		return -1;
	}

	//Check if value is the value requested
	if(receivedBytes[0] == 0x71 && receivedBytes[5] == 0xff && receivedBytes[6] == 0xff && receivedBytes[7] == 0xff){
		value = (receivedBytes[4] << 24) | (receivedBytes[3] << 16) | (receivedBytes[2] << 8) | (receivedBytes[1]);//Little-endian conversion
	}
	return value;
}

/**
 * Sets the text of a component
 * @param compVarString
 * @param text
 * @return
 */
NextionDisplay *NextionDisplay::setCompText(String compVarString, String text)
{
	String newText = compVarString + ".txt=\"" + text + "\"";
	sendCommand(newText.c_str());
	return this;
}

/**
 * Adds a value to the graph (value needs to fit into height of graph)
 * @param waveFormId
 * @param channel
 * @param value
 * @return
 */
NextionDisplay *NextionDisplay::addGraphValue(int waveFormId, int channel, int value)
{
	String graphCommand = "add ";
	graphCommand.concat(waveFormId);
	graphCommand.concat(",");
	graphCommand.concat(channel);
	graphCommand.concat(",");

	//Making sure value is limited to the supported values of the nextion displays.
	if (value >= 255) {
		value = 255;
	} else if (value <= 0) {
		value = 0;
	}

	graphCommand.concat(value);

	sendCommand(string2char(graphCommand));
	return this;
}

/**
 * Helper function to convert a string object to a char array
 * @param command
 * @return
 */
char *NextionDisplay::string2char(const String &command)
{
	char *p = const_cast<char *>(command.c_str());
	return p;
}

/**
 * Checks if a component was clicked, will also modify the pageId and compId value
 * @param _pageId
 * @param compId
 * @return
 */
bool NextionDisplay::getComponentClicked(int &_pageId, int &compId)
{
	String cmd;
	while (Serial2.available()) {
		delay(20);
		char byte = Serial2.read();
		if (byte == 0xff) {
			break;
		}
		cmd += byte;
	}

	switch (cmd[0]) {
		case TOUCH_EVENT:
			_pageId = cmd[1];
			compId = cmd[2];
			return true;
			break;
	}
	return false;
}

/**
 * Reads a full command from the Serial2
 * @return
 */
String NextionDisplay::readCommand()
{
	String cmd;
	while (Serial2.available()) {
		delay(20);
		char byte = Serial2.read();

		if (byte == 0xff) {
			break;
		}
		cmd += byte;
	}
	String cmdConstruct;
	switch (cmd[0]) {
		case TOUCH_EVENT:
			for (int i = 0; i < cmd.length(); i++) {
				if (i >= 3) {
					break;
				}
				cmdConstruct += String(cmd[i], HEX);
				cmdConstruct += " ";
			}
			break;
	}
	return cmdConstruct;
}
