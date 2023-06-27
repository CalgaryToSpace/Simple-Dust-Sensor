#include <Arduino.h>
#include <Wire.h>
#include <SPI.h>
#include <Adafruit_PM25AQI.h>
#include <Adafruit_GFX.h>
#include <Adafruit_LEDBackpack.h>

#include <WiFi.h>
#include <WiFiMulti.h>
#include <HTTPClient.h>
//#include <Arduino_JSON.h>
#include <CayenneArduinoDefines.h>

#include "wifi_credentials.h"

#define CAYENNE_PRINT Serial
#include <CayenneMQTTESP32.h>

// can define any I2C pins
#define PIN_I2C_SDA 18 // (white wire)
#define PIN_I2C_SCL 19 // (green wire)
#define PIN_PM25_RX 16 // PM25 sensor TX connected to GPIO16 (MCU RX Serial2)

Adafruit_PM25AQI aqi = Adafruit_PM25AQI();

void log_pm25_data_to_serial(PM25_AQI_Data data);

Adafruit_7segment matrix_now = Adafruit_7segment();
Adafruit_7segment matrix_max = Adafruit_7segment();
uint16_t max_particle_value = 0;

WiFiMulti wifiMulti;

String serverName = "http://enyxtb00kblnc.x.pipedream.net/pm-sensor";

bool cayenne_begin_yet = false;

void setup() {
	Serial.begin(115200); // debug serial line
	Serial.println("Starting setup()");

	// wait a sec for devices to boot
	delay(1000);

	// init I2C
	Serial.println("Init I2C");
	Wire.begin(PIN_I2C_SDA, PIN_I2C_SCL); // SDA, SCL
	Serial.println("I2C init done");

	// test the I2C 7segs
	matrix_now.begin(0x71); // A0 jumpered
	matrix_max.begin(0x72); // A1 jumpered
	matrix_now.setBrightness(15);
	matrix_max.setBrightness(15);
	matrix_now.print(1111, DEC); matrix_now.writeDisplay();
	matrix_max.print(2222, DEC); matrix_max.writeDisplay();
	Serial.println("7seg test done");

	// init particulate sensor
	Serial.println("Init PM 2.5 sensor");
	Serial2.begin(9600);
	if (! aqi.begin_UART(&Serial2)) {
		Serial.println("Could not find PM 2.5 sensor!");
		while (1) delay(100);
	}
	Serial.println("PM 2.5 sensor found and connected!");

	// setup the wifi
	wifiMulti.addAP(wifi1_ssid, wifi1_password);
	wifiMulti.addAP(wifi2_ssid, wifi2_password);
	wifiMulti.addAP(wifi3_ssid, wifi3_password);
	wifiMulti.addAP(wifi4_ssid, wifi4_password);
  
	Serial.println("Done setup()");
}

void loop() {
	Serial.println("Starting loop()");

	// read data
	PM25_AQI_Data data;
	if (! aqi.read(&data)) {
		Serial.println("Could not read from AQI");
		delay(500);  // try again in a bit!
		return;
	}
	Serial.println("Read from AQI successful.");

	// log data
	log_pm25_data_to_serial(data);

	// update max value // TODO make it a 7-day moving max
	if (data.particles_03um > max_particle_value) {
		max_particle_value = data.particles_03um;
	}
	
	// update the screen
	matrix_now.print(data.particles_03um, DEC); matrix_now.writeDisplay();
	matrix_max.print(max_particle_value, DEC); matrix_max.writeDisplay();

	// try wifi
	if (wifiMulti.run() != WL_CONNECTED) {
		Serial.println("WiFi not connected!");
	}
	else if (wifiMulti.run() == WL_CONNECTED) {
		Serial.print("WiFi connected. IP address: ");
		Serial.print(WiFi.localIP());
		Serial.print("\t\tSSID: ");
		Serial.print(WiFi.SSID());
		Serial.print("\t\tRSSI: ");
		Serial.println(WiFi.RSSI());

		if (!cayenne_begin_yet) {
			Cayenne.begin(cayenne_username, cayenne_password, cayenne_client_id);
			cayenne_begin_yet = true;
		}

		Cayenne.loop();

		Cayenne.virtualWrite(30, data.particles_03um); // Particle > 0.3um / 0.1L air
		Cayenne.virtualWrite(31, data.particles_05um); // Particle > 0.5um / 0.1L air
		Cayenne.virtualWrite(32, data.particles_10um); // Particle > 1.0um / 0.1L air
		Cayenne.virtualWrite(33, data.particles_25um); // Particle > 2.5um / 0.1L air
		Cayenne.virtualWrite(34, data.particles_50um); // Particle > 5.0um / 0.1L air
		Cayenne.virtualWrite(35, data.particles_100um); // Particle > 10 um / 0.1L air

		Cayenne.virtualWrite(6, data.pm10_standard); // PM 1.0 concentration standard (ug/m3)
		Cayenne.virtualWrite(7, data.pm25_standard); // PM 2.5 concentration standard (ug/m3)
		Cayenne.virtualWrite(8, data.pm100_standard); // PM 10 concentration standard (ug/m3)

		Cayenne.virtualWrite(9, data.pm10_env); // PM 1.0 concentration environmental (ug/m3)
		Cayenne.virtualWrite(10, data.pm25_env); // PM 2.5 concentration environmental (ug/m3)
		Cayenne.virtualWrite(11, data.pm100_env); // PM 10 concentration environmental (ug/m3)

		/*

		// send data to server
		HTTPClient http;
		
		// Your Domain name with URL path or IP address with path
		http.begin(serverName.c_str());
		
		// If you need Node-RED/server authentication, insert user and password below
		//http.setAuthorization("REPLACE_WITH_SERVER_USERNAME", "REPLACE_WITH_SERVER_PASSWORD");
		
		// Send HTTP GET request
		int httpResponseCode = http.GET();
		
		if (httpResponseCode>0) {
			Serial.print("HTTP Response code: ");
			Serial.println(httpResponseCode);
			String payload = http.getString();
			Serial.println(payload);
		}
		else {
			Serial.print("Error code: ");
			Serial.println(httpResponseCode);
		}
		// Free resources
		http.end();
		*/
		
	}

	// wait a bit
	delay(1000);

	Serial.println("End loop()");

}

void log_pm25_data_to_serial(PM25_AQI_Data data) {
	Serial.println();
	Serial.println(F("---------------------------------------"));
	Serial.print(F("Concentration Units (standard):      "));
	Serial.print(F("PM 1.0: ")); Serial.print(data.pm10_standard);
	Serial.print(F("\t\tPM 2.5: ")); Serial.print(data.pm25_standard);
	Serial.print(F("\t\tPM 10: ")); Serial.println(data.pm100_standard);
	Serial.print(F("Concentration Units (environmental): "));
	Serial.print(F("PM 1.0: ")); Serial.print(data.pm10_env);
	Serial.print(F("\t\tPM 2.5: ")); Serial.print(data.pm25_env);
	Serial.print(F("\t\tPM 10: ")); Serial.println(data.pm100_env);
	Serial.println(F("---------------------------------------"));
	Serial.print(F("Particles > 0.3um / 0.1L air:")); Serial.println(data.particles_03um);
	Serial.print(F("Particles > 0.5um / 0.1L air:")); Serial.println(data.particles_05um);
	Serial.print(F("Particles > 1.0um / 0.1L air:")); Serial.println(data.particles_10um);
	Serial.print(F("Particles > 2.5um / 0.1L air:")); Serial.println(data.particles_25um);
	Serial.print(F("Particles > 5.0um / 0.1L air:")); Serial.println(data.particles_50um);
	Serial.print(F("Particles > 10 um / 0.1L air:")); Serial.println(data.particles_100um);
	Serial.println(F("---------------------------------------"));
}

CAYENNE_OUT_DEFAULT()
{
	// Write data to Cayenne here. This example just sends the current uptime in milliseconds on virtual channel 0.
	Cayenne.virtualWrite(20, millis() / 1000); // uptime in sec
	Cayenne.virtualWrite(21, millis() / 1000 / 3600); // uptime in hours

}
