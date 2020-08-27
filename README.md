This is an enclosure controller for 3d printers that runs on the dual core ESP32.

- It uses a Nextion NX4832T035 3.5inch 480px320p color touch TFT display.
- It also includes a simple Webinterface made with ESPUI for quick access without having to go to the display.

***This project consists of 4 parts.***
1. The ESP32 software.
2. The Nextion display software (text, icons, images, ...).
3. The PCB.
4. The 3D Printed case to house everything.

***Features of this controller are:***
- 2x temperature and humidity sensors (DHT22).
- 2x 4-pin pwm fans.
- 1x Buzzer for alarms, warnings and such (currently not implemented as of v1.3.1).
- 1x 12v led strip connection controlled by a relay. This is usefull if you want to have a single color (like white).
- 1x 5V WS2812B single addressable led strip

***Software features:***
- ESPUI webinterface
- WifiManager creates a hotspot you can connect to that will make it possible to set the wifi network to connect to on boot.
- All values that can be changed can also be stored to flash. These values will be retrieved at boot and only saved using button pressed to prevent flash wear.
- Displaying of fan speed and percent.
- The possibility to change the speed of fans from 0% - 100%
- Displaying of sensor humidity and temperature
- Switching of 12v led strip.
- Graph for temperatures (only nextion display currently)
- Multiple modes choosable for WS2812B
  - (rainbow, fate, pulse, temperature (led strip is split in half showing temperature in color), running)
- Multiple colors choosable for WS2812B
- LCD Brighness
- LCD Sleep
- Temperature Warning and Danger 
  - (Currently do the same)
  - At some point Danger warning is supposed to do everything to make your notice something going wrong.
  - Like buzzer alarm, led blinking, display blinking and such. This is supposed to be used when something is seriously wrong > xyzTemperature.

***Notices:***
- The board uses a XL4015 buck converter which is rated for ~5A of power. 
- Make sure to choose an led strip that won't overload the converter. As this could lead to overheating.
- Also make sure to leave a bit of room for all other components like fans, 12v led strip, sensors and display.

- It is expecially important to make sure the Nextion display gets enough power.
- If your display starts showing weird graphics. DISCONNECT power immediately.
- Don't run the pcb from 5V pc usb port.
- Low current can destroy the display (although it never happened to me).

- Fan rpm could be off a bit as tacho reading is influenced by noise.
- The tacho ports both have an rc filter and most fans will work without a problem and will display the correct value.
- I have one fan (NF-F12 industrialPPC-3000) that will always display > 4500rpm. 
  - (should only have 3000rpm). 
  - I'm not sure if that is something caused by the fan itself.

***Credits:***
- Icons shown on the Nextion display are from https://icons8.com/
