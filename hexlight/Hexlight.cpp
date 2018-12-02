#include "Hexlight.h"

Hexlight::Hexlight(Adafruit_NeoPixel& strip) : HexlightWifi() {
	this->initOTA("OTApass");
	this->debug("Hexlight - Setup");
	this->strip = strip;
}

Hexlight::~Hexlight() {
	this->debug("Hexlight shutting down!");
}

void Hexlight::hexDelay(unsigned long ms) {
	for(int i = 0; i < ms; i++) {
		delay(1);
		if(this->delayHandle()) {
			return;
		}
	}	
}

bool Hexlight::delayHandle() {
	this->handleWifi();
	ArduinoOTA.handle();
	if(this->newPacket) {
		return true;
	}
	return false;
}

void Hexlight::handle() {
	this->handleWifi();
	ArduinoOTA.handle();
	if(this->newPacket) {
		this->readPacket();
	}
	this->handlePattern();
}

void Hexlight::readPacket() {
	switch(this->incomingPacket[0]) {
		case '!':
				this->setPattern(2);
			break;
		case '*':
				this->setPattern(1);
			break;
		case '+':
				this->setPattern(3);
			break;
		case '#':
				this->parseColor();
				this->setPattern(4);
			break;
		case '-':
				this->parseColor();
				this->parseTime(9);
				this->setPattern(5);
			break;
		case 'd':
				this->setPattern(0);
			break;
		case '?':
				this->parseTime(1);
				this->setPattern(6);
			break;
	}
}

void Hexlight::parseColor() {
	int color = strtoll( &incomingPacket[1], NULL, 16);
	Serial.printf("[!] color: %#08x\r\n", color);
	uint8_t r = color >> 24;
	uint8_t g = color >> 16 & 0xFF;
	uint8_t b = color >> 8 & 0xFF;
	uint8_t w = color & 0xFF;
	Serial.printf("[!] red: %#04x\r\n", r);
	Serial.printf("[!] green: %#04x\r\n", g);
	Serial.printf("[!] blue: %#04x\r\n", b);
	Serial.printf("[!] white: %#04x\r\n", w);
	
	this->color = this->strip.Color(r, g, b, w);
}

uint32_t Hexlight::parseTime(uint8_t pos) {
	if(incomingPacket[pos] == ',') {
		this->time = strtoll(&incomingPacket[pos], NULL, 10);
	}
	else {
		this->time = 10;
	}
}

void Hexlight::setPattern(uint8_t pattern) {
	this->pattern = pattern;
	this->step = 0;
	this->resetPacket();
}

void Hexlight::handlePattern() {
	switch(this->pattern) {
		case 0:
				this->demo();
			break;
		case 1:
				this->colorWipe(this->strip.Color(0, 0, 0, 0), 0);
			break;
		case 2:
				this->rainbowCycle(1);
			break;
		case 3:
				this->candle();
			break;
		case 4:
				this->colorWipe(this->color, 0);
			break;
		case 5:
				this->flash(this->color, this->time);
			break;
		case 6:
				this->rainbow(this->time);
			break;
		default:
				this->demo();
			break;
	}
	if(this->step > 256) {
		this->step = 0;
		this->demoStep++;
	}
}

void Hexlight::demo() {
	//this->rainbow(250);
	//Serial.println("DEMO!");
	switch(this->demoStep) {
		case 0:
				this->colorWipe(strip.Color(255, 0, 0, 0), 50);
				this->hexDelay(1000);
				this->demoStep++;
			break;
		case 1:
				this->colorWipe(strip.Color(0, 255, 0, 0), 50);
				this->hexDelay(1000);
				this->demoStep++;
			break;
		case 2:
				this->colorWipe(strip.Color(0, 0, 255, 0), 50);
				this->hexDelay(1000);
				this->demoStep++;
			break;
		case 3:
				this->colorWipe(strip.Color(0, 0, 0, 255), 50);
				this->hexDelay(1000);
				this->demoStep++;
			break;
		case 4:
				this->colorWipe(strip.Color(255, 255, 255, 255), 50);
				this->hexDelay(1000);
				this->demoStep++;
			break;
		case 5:
				this->rainbowCycle(20);
			break;
		case 6:
				this->rainbow(20);
			break;
		//case 7:
		//		this->singleLedRainbow(100);
		//	break;
		//case 8:
		//		this->theaterChase(this->strip.Color(0, 255, 0, 0));
		//	break;
		default:
				this->demoStep = 0;
			break;
	}
}

/**************
**					 **
**	PATTERN	**
**					 **
***************/
void Hexlight::rainbow(uint8_t wait) {
	uint16_t i;
	
	for (i = 0; i < this->strip.numPixels(); i++) {
		this->strip.setPixelColor(i, this->wheel((i + this->step) & 255));
	}
	this->strip.show();
	this->hexDelay(wait);
	this->step++;
}

void Hexlight::singleLedRainbow(uint8_t wait) {
	for(int i = 0; i < this->strip.numPixels(); i++)
	this->strip.setPixelColor(this->step % this->strip.numPixels(), this->wheel(this->step));
	this->hexDelay(wait);
	if(this->step > 255) {
		this->step = 0;
	}
}

void Hexlight::theaterChase(uint32_t c, uint8_t wait) {
	for (int q = 0; q < 3; q++) {
		for (uint16_t i = 0; i < this->strip.numPixels(); i = i + 3) {
			this->strip.setPixelColor(i + 1, c);	//turn every third pixel on
		}
		strip.show();
		this->hexDelay(wait);
		
		for (uint16_t i = 0; i < this->strip.numPixels(); i = i + 3) {
			this->strip.setPixelColor(i + 1, 0);			//turn every third pixel off
		}
		this->strip.show();
	}
	this->step++;
}

void Hexlight::theaterChaseRainbow(uint8_t wait, uint8_t steps) {
	for (int q = 0; q < 3; q++) {
		uint8_t s = random(3,10);
		for (uint16_t i=0; i < this->strip.numPixels(); i=i+s) {
			this->strip.setPixelColor(i+q, wheel( (i+this->step) % 255) );		//turn every third pixel on
		}
		strip.show();
		this->hexDelay(wait);
		
		for (uint16_t i=0; i < this->strip.numPixels(); i=i+s) {
			this->strip.setPixelColor(i+q, 0);				//turn every third pixel off
		}
		this->strip.show();
	}
	this->step++;
}

void Hexlight::candle() {
	uint8_t green; // brightness of the green 
	uint8_t red;	// add a bit for red
	green = 50 + random(155);
	red = green + random(50);
	this->strip.setPixelColor(random(strip.numPixels() - 1), red, green, 0);
	this->strip.show();
	this->step++;
	this->hexDelay(5);
}

//stepped ... only turn lights on waits and turns the off stepping is not required
void Hexlight::flash(uint32_t c, uint8_t wait) {
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
	this->step++;
}

//stepped
void Hexlight::colorSet(uint32_t c) {
	for (uint16_t i = 0; i < this->strip.numPixels(); i++) {
		this->strip.setPixelColor (i, c);
	}
	this->strip.show();
}

//stepped
void Hexlight::rainbowCycle(uint8_t wait) {
	//this->debug("RainbowCycle - Method");
	uint16_t i;
	for (i = 0; i < this->strip.numPixels(); i++) {
		this->strip.setPixelColor(i, this->wheel(((i * 256 / this->strip.numPixels()) + this->step) & 255));
	}
	this->strip.show();
	this->step++;
	this->hexDelay(wait);
}

uint32_t Hexlight::wheel(byte WheelPos) {
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

//stepped
void Hexlight::colorWipe(uint32_t c, uint8_t wait) {
	for (uint16_t i = 0; i < this->strip.numPixels(); i++) {
		this->strip.setPixelColor (i, c);
		this->strip.show();
		this->step++;
		this->hexDelay(wait);
	}
}
/***************************
**                        **
**          INIT          **
**                        **
***************************/
void Hexlight::initOTA(char* OTApass) {
	// OTA Port defaults to 8266
	ArduinoOTA.setPort(8266);

	// Hostname defaults to esp8266-[ChipID]
	// Todo: Name with last octett of IP?
	ArduinoOTA.setHostname("Hexlight");

	// No authentication by default
	ArduinoOTA.setPassword((const char *)"totalsecurity");

	this->initOnError();
	this->initOnStart();
	this->initOnEnd();
	this->initOnProgress();
	
	ArduinoOTA.begin();
}

void Hexlight::initOnError() {
	ArduinoOTA.onError([&](ota_error_t error) {
		Serial.printf("[-] Error[%u]: ", error);
		
		// check error type
		if (error == OTA_AUTH_ERROR) {
			this->debug("Auth Failed");
		}
		else if (error == OTA_BEGIN_ERROR) {
			this->debug("Begin Failed");
		}
		else if (error == OTA_CONNECT_ERROR) {
			this->debug("Connect Failed");
		}
		else if (error == OTA_RECEIVE_ERROR) {
			this->debug("Receive Failed");
		}
		else if (error == OTA_END_ERROR) {
			this->debug("End Failed");
		}
		this->flash(strip.Color(255, 0, 0, 0), 150);
	});
}

void Hexlight::initOnStart() {
	ArduinoOTA.onStart([&]() {
		this->debug("[+] Starting OTA flashing");
		this->colorWipe(strip.Color(255, 0, 0, 0), 0);
	});
}

void Hexlight::initOnEnd() {
	ArduinoOTA.onEnd([&]() {
		this->debug("[+] OTA has finished");
		this->flash(strip.Color(0, 255, 0, 0), 150);
	});
}

void Hexlight::initOnProgress() {
	ArduinoOTA.onProgress([&](unsigned int progress, unsigned int total) {
		Serial.printf("[*] Progress: %u%%\r\n", (progress / (total / 100)));
		// slowly fade from red to green
		this->colorSet(strip.Color(255 - ((progress / (total / 100)) * 2.55), 0 + ((progress / (total / 100)) * 2.55), 0, 0));
	});
}
