An enclosure controller for 3d printers that runs on the dual core ESP32.

It uses as Nextion NX4832T035 3.5inch 480px320p color touch TFT display.
It also includes a simple Webinterface made with ESPUI for quick access without having to go to the display.

This makes it possible to use:
2x temperature and humidity sensors (DHT22).
2x 4-pin pwm fans. The speed of the fans can be changed and speed is measured in rpm to be displayable.

2x LED strips:
One is for 12V led strips (for exaple just white).
One is for 5V WS2812B adressable led strips.

A buzzer can be added to get errors (like wifi connection failed) without having to look at the screen/webinterface.


Credits:
Icons shown on the Nextion display are from https://icons8.com/
