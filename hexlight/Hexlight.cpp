#include "Hexlight.h"

Hexlight::Hexlight(uint8_t pin, uint8_t numLeds, char* OTApass) : HexlightWifi() {
	this->initOTA(OTApass);
	this->initStrip(pin, numLeds);
	this->Udp.begin(PORT);
	this->debug("Hexlight - Setup");
}

void Hexlight::hexDelay(unsigned long ms) {
	for(int i = 0; i < ms; i++) {
		Serial.printf("Delay Loop - %i\n", i);
		delay(1);
		//ms -= this->handle();
	}	
}

void Hexlight::handle() {
	this->handleWifi();
	ArduinoOTA.handle();
	this->hexParsePacket();
}

void Hexlight::doStep() {
	switch(this->pattern) {
		case 1:
			{
				int a = 1;
				while(incomingPacket[a] != 0) {
					this->parseColor(a);
					a += 2;
				}
				this->colorWipe(this->strip.Color(this->red, this->green, this->blue, this->white), 50);
			break;
			}
		case 2:
				this->strip.setPixelColor (this->incomingPacket[1], this->strip.Color(this->incomingPacket[2], this->incomingPacket[3], this->incomingPacket[4], this->incomingPacket[5]));
				this->strip.show();
			break;
		case 3:
				this->rainbowCycle(10);
			break;
		case 4:
				this->candle();
			break;
	}
}

void Hexlight::hexParsePacket() {
	int packetSize = this->Udp.parsePacket();
	
	if (packetSize) {
		Serial.printf("Received %d bytes from %s, port %d\n", packetSize, this->Udp.remoteIP().toString().c_str(), this->Udp.remotePort());
		int len = this->Udp.read(this->incomingPacket, 255);
		if (len > 0) {
			this->incomingPacket[len] = 0;
		}
		Serial.printf("Udp packet contents: %s\n", this->incomingPacket);
		
		if(this->incomingPacket[0] == '*') {
			this->debug("Colorwipe");
			this->pattern = 1;
		}
		else if(this->incomingPacket[0] == '#') {
			this->debug("Setting single LED");
			this->pattern = 2;
		}
		else if(this->incomingPacket[0] == 'p') {
			if(this->incomingPacket[1] == 'r') {
				this->debug("RainbowCycle - Parsing");
				this->pattern = 3;
				this->resetPacket();
				delay(2000);
			}
			else if(this->incomingPacket[1] == 'c') {
				this->debug("Candle");
				this->pattern = 4;
			}
		}
	}
}

void Hexlight::resetPacket() {
	for(int i = 0; i < 255; i++) {
		this->incomingPacket[i] = 0;
	}
	this->lastStep = 0;
	this->step = 0;
	this->debug("reset");
}

void Hexlight::parseColor(int i) {
	switch(this->incomingPacket[i]) {
		case 98: // b
		case 66: // B
			this->blue = this->incomingPacket[i+1];
			break;
		case 82:	// R
		case 114: // r
			this->red = this->incomingPacket[i+1];
			break;
		case 103: // g
		case 71:	// G
			this->green = this->incomingPacket[i+1];
			break;
		case 119: // w
		case 87:	// W
			this->white = this->incomingPacket[i+1];
			break;
		case 120: //x
		case 88:	//X
			this->red = 0;
			this->green = 0;
			this->blue = 0;
			this->white = 0;
			break;
	}
}

/**************
**           **
**  PATTERN  **
**           **
***************/

void Hexlight::candle() {
	this->debug("Candle");
	uint8_t green; // brightness of the green 
	uint8_t red;  // add a bit for red
	for(uint8_t i=0; i<100; i++) {
		green = 50 + random(155);
		red = green + random(50);
		this->strip.setPixelColor(random(strip.numPixels() - 1), red, green, 0);
		this->strip.show();
		this->hexDelay(5);
		
	}
}

void Hexlight::flash(uint32_t c, uint8_t t, uint8_t wait) {
	for(uint8_t i = 0; i < t; i++) {
		for (uint8_t o = 0; o < this->strip.numPixels(); o++) {
			this->strip.setPixelColor (o, c);
		}
		this->strip.show();
		this->hexDelay(wait);
		for (uint8_t o = 0; o < this->strip.numPixels(); o++) {
			this->strip.setPixelColor(o, this->strip.Color(0, 0, 0, 0));
		}
		this->strip.show();
		this->hexDelay(wait);
		
	}
}

void Hexlight::colorSet(uint32_t c) {
	for (uint16_t i = 0; i < this->strip.numPixels(); i++) {
		this->strip.setPixelColor (i, c);
	}
	this->strip.show();
}

void Hexlight::rainbowCycle(uint8_t wait) {
	uint16_t i, j;
	unsigned long current = millis();
	unsigned long currentWait = current - this->lastStep;
	if(current - this->lastStep >= wait) {
		Serial.printf("RainbowCycle - Wait was %i\n", currentWait);
		for (i = 0; i < this->strip.numPixels(); i++) {
			this->strip.setPixelColor(i, this->Wheel(((i * 256 / this->strip.numPixels()) + this->step) & 255));
		}
		this->strip.show();
		
		this->lastStep = millis();
		this->step += 1;
		Serial.printf("Current Step is %i\n", this->step);
	}
}

uint32_t Hexlight::Wheel(byte WheelPos) {
	WheelPos = 255 - WheelPos;
	if (WheelPos < 85) {
		return this->strip.Color(255 - WheelPos * 3, 0, WheelPos * 3);
	}
	if (WheelPos < 170) {
		WheelPos -= 85;
		return this->strip.Color(0, WheelPos * 3, 255 - WheelPos * 3);
	}
	WheelPos -= 170;
	return this->strip.Color(WheelPos * 3, 255 - WheelPos * 3, 0);
}

void Hexlight::colorWipe(uint32_t c, uint8_t wait) {
	for (uint16_t i = 0; i < this->strip.numPixels(); i++) {
		this->strip.setPixelColor (i, c);
		this->strip.show();
		this->hexDelay(wait);
		
	}
}

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