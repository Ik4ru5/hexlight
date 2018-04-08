#include "Hexlight.h"

Hexlight::Hexlight() {};

Hexlight::Hexlight(uint8_t pin, uint8_t numLeds, char* OTApass) : HexlightWifi() {
	this->initOTA(OTApass);
	this->initStrip(pin, numLeds);
	this->step = 0;
	this->debug("Hexlight - Setup");
}

Hexlight::~Hexlight() {
	Serial.println("Hexlight shutting down!");
}

void Hexlight::hexDelay(unsigned long ms) {
	for(int i = 0; i < ms; i++) {
		Serial.printf("Delay Loop - %i\n", i);
		delay(1);
		this->handle();
	}	
}

void Hexlight::handle() {
	this->handleWifi();
	ArduinoOTA.handle();
	this->hexParsePacket();
}

void Hexlight::doStep() {
}

void Hexlight::hexParsePacket() {
}

void Hexlight::resetPacket() {
/*
	for(int i = 0; i < 255; i++) {
		this->incomingPacket[i] = 0;
	}
	this->lastStep = 0;
	this->step = 0;
	this->debug("reset");
*/
}



/**************
**           **
**  PATTERN  **
**           **
***************/

/**************
**           **
**    INIT   **
**           **
***************/

void Hexlight::initStrip(uint8_t pin, uint8_t numLeds) {
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
}

void Hexlight::initOTA(char* OTApass) {
	// OTA Port defaults to 8266
	ArduinoOTA.setPort(8266);

	// Hostname defaults to esp8266-[ChipID]
	// Todo: Name with last octett of IP?
	ArduinoOTA.setHostname("hexlight");

	// No authentication by default
	// ArduinoOTA.setPassword((const char *)"123");

	this->initOnError();
	this->initOnStart();
	this->initOnEnd();
	this->initOnProgress();
	
	ArduinoOTA.begin();
}

void Hexlight::initOnError() {
	ArduinoOTA.onError([](ota_error_t error) {
		Serial.printf("[-] Error[%u]: ", error);
		
		// check error type
		if (error == OTA_AUTH_ERROR) {
			Serial.println("Auth Failed");
		}
		else if (error == OTA_BEGIN_ERROR) {
			Serial.println("Begin Failed");
		}
		else if (error == OTA_CONNECT_ERROR) {
			Serial.println("Connect Failed");
		}
		else if (error == OTA_RECEIVE_ERROR) {
			Serial.println("Receive Failed");
		}
		else if (error == OTA_END_ERROR) {
			Serial.println("End Failed");
		}
		// TODO: flash(strip.Color(255, 0, 0, 0), 5, 150);
	});
}

void Hexlight::initOnStart() {
	ArduinoOTA.onStart([]() {
		Serial.println("[+] Starting OTA flashing");
		// TODO: colorWipe(strip.Color(255, 0, 0, 0), 0);
	});
}

void Hexlight::initOnEnd() {
	ArduinoOTA.onEnd([]() {
		Serial.println("[+] OTA has finished");
		// TODO: flash(strip.Color(0, 255, 0, 0), 5, 150);
	});
}

void Hexlight::initOnProgress() {
	ArduinoOTA.onProgress([](unsigned int progress, unsigned int total) {
		Serial.printf("[*] Progress: %u%%\r\n", (progress / (total / 100)));
		// TODO :colorSet(strip.Color(255 - ((progress / (total / 100)) * 2.55), 0 + ((progress / (total / 100)) * 2.55), 0, 0));
	});
}