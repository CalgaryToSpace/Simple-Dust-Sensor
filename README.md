# Simple-Dust-Sensor
A simple dust sensor to monitor the clean room, utilizing 7-seg displays and a wifi-connected microcontroller

## Software Development
* This project utilizes PlatformIO, an extension for VS Code. Install PlatformIO and open the VS Code workspace file in this repo to get started.
* Duplicate the `include/wifi_credentials.sample.h` to `include/wifi_credentials.h`, and fill the credentials to maintain wifi connectivity.
* The microcontroller is an ESP32 devkit, and can be programmed over USB from PlatformIO.

## Wifi Interface
* This project displays the current and max dust values on the device itself, and also relays them over a network to view logs/a graph over time.

## Hardware BOM
* [PM2.5 Air Quality Sensor and Breadboard Adapter Kit - PMS5003](https://www.adafruit.com/product/3686)
	* [Arduino Tutorial](https://learn.adafruit.com/pm25-air-quality-sensor/arduino-code)
	* Connect TX to GPIO16 (Serial2 RX)
* esp32doit-devkit-v1 Microcontroller Dev Kit
	* [Pin Usage Reference](https://randomnerdtutorials.com/esp32-pinout-reference-gpios/)
* [Adafruit 0.56" 7-segment LED HT16K33 Backpack](https://learn.adafruit.com/adafruit-led-backpack/0-dot-56-seven-segment-backpack)
	* [Arduino Tutorial](https://learn.adafruit.com/adafruit-led-backpack/0-dot-56-seven-segment-backpack-arduino-setup)

## Credits
* [mrWheel's YAPP Box](https://github.com/mrWheel/YAPP_Box) for the enclosure
* Arduino libraries
