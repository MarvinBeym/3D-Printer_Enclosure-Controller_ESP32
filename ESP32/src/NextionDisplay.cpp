#include <Arduino.h>
#include "NextionDisplay.h"

//Extracted from https://nextion.tech/instruction-set/
const char NEXTION_STARTUP = 0x00;
const char SERIAL_BUFFER_OVERFLOW = 0x24;
const char TOUCH_EVENT = 0x65;
const char CURRENT_PAGE_NUMBER = 0x66;
const char TOUCH_COORDINATE_AWAKE = 0x67;
const char TOUCH_COORDINATE_SLEEP = 0x68;
const char STRING_DATA = 0x70;
const char NUMBER_DATA = 0x71;
const char AUTO_ENTERED_SLEEP = 0x86;
const char AUTO_WAKE_FROM_SLEEP = 0x87;
const char NEXTION_READY = 0x88;

NextionDisplay::NextionDisplay(HardwareSerial &hwSerial, int baudRate)
{
	pageId = MAIN_PAGE;
	//hwSerial->begin(baudRate);
}

void NextionDisplay::setPage(int pageId)
{
	this->pageId = pageId;
	String cmd = "page " + String(pageId);
	this->sendCommand(cmd.c_str());
}

void NextionDisplay::sendCommand(const char *cmdToSend)
{
	while (Serial2.available())
	{
		Serial2.read();
	}

	Serial2.print(cmdToSend);
	Serial2.write(0xff);
	Serial2.write(0xff);
	Serial2.write(0xff);
}

void NextionDisplay::getComponentClicked(int &pageId, int &compId)
{
	String cmd;
	while (Serial2.available())
	{
		delay(20);
		char byte = Serial2.read();
		if (byte == 0xff)
		{
			break;
		}
		cmd += byte;
	}

	switch (cmd[0])
	{
	case TOUCH_EVENT:
		pageId = cmd[1];
		compId = cmd[2];
		break;
	}
}

String NextionDisplay::readCommand()
{
	String cmd;
	while (Serial2.available())
	{
		delay(20);
		char byte = Serial2.read();

		if (byte == 0xff)
		{
			break;
		}
		cmd += byte;
	}
	String cmdConstruct;
	switch (cmd[0])
	{
	case TOUCH_EVENT:
		for (int i = 0; i < cmd.length(); i++)
		{
			if (i >= 3)
			{
				break;
			}
			cmdConstruct += String(cmd[i], HEX);
			cmdConstruct += " ";
		}
		break;
	}
	return cmdConstruct;
}