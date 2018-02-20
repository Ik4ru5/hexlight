#include "Hexlight.h"

Hexlight::Hexlight(uint8_t pin, uint8_t numLeds) : HexlightWifi() {
	// Parameter 1 = number of pixels in strip
	// Parameter 2 = Arduino pin number (most are valid)
	// Parameter 3 = pixel type flags, add together as needed:
	//	 NEO_KHZ800	800 KHz bitstream (most NeoPixel products w/WS2812 LEDs)
	//	 NEO_KHZ400	400 KHz (classic 'v1' (not v2) FLORA pixels, WS2811 drivers)
	//	 NEO_GRB		 Pixels are wired for GRB bitstream (most NeoPixel products)
	//	 NEO_RGB		 Pixels are wired for RGB bitstream (v1 FLORA pixels, not v2)
	//	 NEO_RGBW		Pixels are wired for RGBW bitstream (NeoPixel RGBW products)
	this->strip = Adafruit_NeoPixel(numLeds, pin, NEO_GRBW + NEO_KHZ800);
	
	// IMPORTANT: To reduce NeoPixel burnout risk, add 1000 uF capacitor across
	// pixel power leads, add 300 - 500 Ohm resistor on first pixel's data input
	// and minimize distance between Arduino and first pixel.	Avoid connecting
	// on a live circuit...if you must, connect GND first.
	
	// Init LED Strip
	this->strip.begin();
	// clear LED Strip
	this->strip.show();
	
	// OTA Port defaults to 8266
	ArduinoOTA.setPort(8266);

	// Hostname defaults to esp8266-[ChipID]
	// Todo: Name with last octett of IP?
	ArduinoOTA.setHostname("hexlight");

	// No authentication by default
	// ArduinoOTA.setPassword((const char *)"123");
	ArduinoOTA.onStart([]() {
		Serial.println("[+] Starting OTA flashing");
		// TODO: colorWipe(strip.Color(255, 0, 0, 0), 0);
	});
	ArduinoOTA.onEnd([]() {
		Serial.println("[+] OTA has finished");
		// TODO: flash(strip.Color(0, 255, 0, 0), 5, 150);
	});
	ArduinoOTA.onProgress([](unsigned int progress, unsigned int total) {
		Serial.printf("[*] Progress: %u%%\r\n", (progress / (total / 100)));
		// TODO :colorSet(strip.Color(255 - ((progress / (total / 100)) * 2.55), 0 + ((progress / (total / 100)) * 2.55), 0, 0));
	});
	ArduinoOTA.onError([](ota_error_t error) {
		Serial.printf("[-] Error[%u]: ", error);
		if (error == OTA_AUTH_ERROR) Serial.println("Auth Failed");
		else if (error == OTA_BEGIN_ERROR) Serial.println("Begin Failed");
		else if (error == OTA_CONNECT_ERROR) Serial.println("Connect Failed");
		else if (error == OTA_RECEIVE_ERROR) Serial.println("Receive Failed");
		else if (error == OTA_END_ERROR) Serial.println("End Failed");
		// TODO: flash(strip.Color(255, 0, 0, 0), 5, 150);
	});
	ArduinoOTA.begin();
}

void Hexlight::delay(unsigned long ms) {
	uint16_t start = (uint16_t)micros();

	while (ms > 0) {
		yield();
		if (((uint16_t)micros() - start) >= 1000) {
			ms--;
			start += 1000;
			this->handle();
			ArduinoOTA.handle();
		}
	}
}