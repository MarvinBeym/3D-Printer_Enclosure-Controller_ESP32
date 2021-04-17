/*
  Class for dealing with the nextion display
  Created by Marvin Beym, 03.04.2021.
*/
#pragma once

//Page definition
#define BOOT_PAGE 0
#define MAIN_PAGE 1
#define FANS_PAGE 2
#define SENSOR_PAGE 3
#define LED_PAGE 4
#define CONF_PAGE 5
#define ABOUT_PAGE 6
#define MESSAGE_PAGE 7

#include <Arduino.h>

class NextionDisplay
{
	public:
		NextionDisplay(int bootDelay, int baudRate, int rxPin, int txPin, EventGroupHandle_t _eg, int _compClickedEvent, void (*_compClickedCallback)(void *));
		String readCommand();
		void setPage(int _pageId);
		void sendCommand(const char *cmdToSend);
		NextionDisplay *setCompValue(String compVarString, int value);
		NextionDisplay *addGraphValue(int waveFormId, int channel, int value);
		int getCompValue(String compVarString);
		char *string2char(const String &compVarString);

		NextionDisplay *setCompText(String compVarString, String text);
		int currentPage;
		int compClicked_pageId = -1;
		int compClicked_compId = -1;
	private:
		bool getComponentClicked(int &pageId, int &compId);
		void (*compClickedCallback)(void *);
		EventGroupHandle_t eg;
		int compClickedEvent;
		static void taskHandler(void *parameter);
		void checkComponentClicked();
};