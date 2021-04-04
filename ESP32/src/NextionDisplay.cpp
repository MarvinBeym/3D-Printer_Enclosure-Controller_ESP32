#include <Arduino.h>
#include "NextionDisplay.h"

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

NextionDisplay::NextionDisplay(HardwareSerial &hwSerial, int baudRate)
{
	pageId = BOOT_PAGE;
	//hwSerial->begin(baudRate);
}

void NextionDisplay::begin(int bootDelay)
{
	setPage(BOOT_PAGE);
	delay(bootDelay);
	setPage(MAIN_PAGE);
}

void NextionDisplay::setPage(int _pageId)
{
	this->pageId = _pageId;
	String cmd = "page " + String(_pageId);
	this->sendCommand(cmd.c_str());
}

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

NextionDisplay *NextionDisplay::setCompValue(String compVarString, int value)
{
	String newValue = compVarString + ".val=" + value;
	sendCommand(newValue.c_str());
	return this;
}

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
		value = (receivedBytes[4] << 24) | (receivedBytes[3] << 16) | (receivedBytes[2] << 8) | (receivedBytes[1]);//Little-endian convertion
	}
	return value;
}

NextionDisplay *NextionDisplay::setCompText(String compVarString, String text)
{
	String newText = compVarString + ".txt=\"" + text + "\"";
	sendCommand(newText.c_str());
	return this;
}

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

char *NextionDisplay::string2char(const String &command)
{
	char *p = const_cast<char *>(command.c_str());
	return p;
}

void NextionDisplay::getComponentClicked(int &pageId, int &compId)
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
			pageId = cmd[1];
			compId = cmd[2];
			break;
	}
}

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
