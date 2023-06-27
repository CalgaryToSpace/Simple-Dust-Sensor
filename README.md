# Simple-Dust-Sensor
A simple dust sensor to monitor the clean room, utilizing 7-seg displays and a wifi-connected microcontroller

## Software Development
* This project utilizes PlatformIO, an extension for VS Code. Install PlatformIO and open the VS Code workspace file in this repo to get started.
* Duplicate the `includes/wifi_credentials.sample.h` to `includes/wifi_credentials.h`, and fill the credentials to maintain wifi connectivity.
* The microcontroller is an ESP32 devkit, and can be programmed over USB from PlatformIO.

## Wifi Interface
* This project displays the current and max dust values on the device itself, and also relays them over a network to view logs/a graph over time.

## Hardware BOM
* [https://www.adafruit.com/product/3686](PM2.5 Air Quality Sensor and Breadboard Adapter Kit - PMS5003)
* esp32doit-devkit-v1 Microcontroller Dev Kit
* 

## Credits
* [mrWheel's YAPP Box](https://github.com/mrWheel/YAPP_Box) for the enclosure
* Arduino libraries
