#include <Arduino.h>
#include <Wire.h>
#include <SPI.h>
#include <Adafruit_PM25AQI.h>

// can define any I2C pins
#define PIN_I2C_SDA 18
#define PIN_I2C_SCL 19
#define PIN_PM25_RX 16 // PM25 sensor TX connected to GPIO16 (MCU RX Serial2)

Adafruit_PM25AQI aqi = Adafruit_PM25AQI();

void log_pm25_data_to_serial(PM25_AQI_Data data);

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
	// FIXME

	// init particulate sensor
	Serial.println("Init PM 2.5 sensor");
	Serial2.begin(9600);
	if (! aqi.begin_UART(&Serial2)) {
		Serial.println("Could not find PM 2.5 sensor!");
		while (1) delay(100);
	}
	Serial.println("PM 2.5 sensor found and connected!");


  
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

	// TODO update on screen

	// wait a bit
	delay(1000);

	Serial.println("End loop()");

}

void log_pm25_data_to_serial(PM25_AQI_Data data) {
	Serial.println();
	Serial.println(F("---------------------------------------"));
	Serial.println(F("Concentration Units (standard)"));
	//Serial.println(F("---------------------------------------"));
	Serial.print(F("PM 1.0: ")); Serial.print(data.pm10_standard);
	Serial.print(F("\t\tPM 2.5: ")); Serial.print(data.pm25_standard);
	Serial.print(F("\t\tPM 10: ")); Serial.println(data.pm100_standard);
	Serial.println(F("Concentration Units (environmental)"));
	//Serial.println(F("---------------------------------------"));
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