
#include <WiFiUdp.h>

#include "Hexlight.h"

#define PIN 0
#define NUM_LEDS 18

WiFiUDP Udp;
unsigned int localUdpPort = 2323;
char incomingPacket[255];
byte red = 0;
byte green = 0;
byte blue = 0;
byte white = 0;
Hexlight *hex;

void setup() {
	hex = new Hexlight(PIN, NUM_LEDS);
	
	Udp.begin(localUdpPort);
}

void loop() {
	hex->handle();

	int packetSize = Udp.parsePacket();
	if (packetSize) {
		Serial.printf("Received %d bytes from %s, port %d\n", packetSize, Udp.remoteIP().toString().c_str(), Udp.remotePort());
		int len = Udp.read(incomingPacket, 255);
		if (len > 0) {
			incomingPacket[len] = 0;
		}
		Serial.printf("UDP packet contents: %s\n", incomingPacket);
		if(incomingPacket[0] == '*') {
			for(int i = 1; i < len; i++) {
			parseColor(incomingPacket, i);
			i++;
			}
			colorWipe(strip.Color(red, green, blue, white), 50);
		}
		else if(incomingPacket[0] == '#') {
			strip.setPixelColor (incomingPacket[1], strip.Color(incomingPacket[2], incomingPacket[3], incomingPacket[4], incomingPacket[5]));
			strip.show();
		}
		else if(incomingPacket[0] == 'p') {
			if(incomingPacket[1] == 'r') {
			rainbowCycle(incomingPacket[2]);
			}
		}
	}
}


void parseColor(char* incomingPacket, int i) {
	switch(incomingPacket[i]) {
	case 98: // b
	case 66: // B
		blue = incomingPacket[i+1];
		break;
	case 82:	// R
	case 114: // r
		red = incomingPacket[i+1];
		break;
	case 103: // g
	case 71:	// G
		green = incomingPacket[i+1];
		break;
	case 119: // w
	case 87:	// W
		white = incomingPacket[i+1];
		break;
	case 120: //x
	case 88:	//X
		red = 0;
		green = 0;
		blue = 0;
		white = 0;
		break;
	}
}

// Fill the dots one after the other with a color
void colorWipe(uint32_t c, uint8_t wait) {
	for (uint16_t i = 0; i < strip.numPixels(); i++) {
		strip.setPixelColor (i, c);
		strip.show();
		delay(wait);
	}
}

void colorSet(uint32_t c) {
	for (uint16_t i = 0; i < strip.numPixels(); i++) {
		strip.setPixelColor (i, c);
	}
	strip.show();
}

void flash(uint32_t c, uint8_t t, uint8_t wait) {
	for(uint8_t i = 0; i < t; i++) {
		for (uint16_t o = 0; o < strip.numPixels(); o++) {
			strip.setPixelColor (o, c);
		}
		strip.show();
		delay(wait);
		for (uint16_t o = 0; o < strip.numPixels(); o++) {
			strip.setPixelColor (o, strip.Color(0, 0, 0, 0));
		}
		strip.show();
		delay(wait);
	} 
}

// Slightly different, this makes the rainbow equally distributed throughout
void rainbowCycle(uint8_t wait) {
	uint16_t i, j;

	for (j = 0; j < 256 * 5; j++) { // 5 cycles of all colors on wheel
		for (i = 0; i < strip.numPixels(); i++) {
			strip.setPixelColor(i, Wheel(((i * 256 / strip.numPixels()) + j) & 255));
		}
		strip.show();
		delay(wait);
	}
}

// Input a value 0 to 255 to get a color value.
// The colours are a transition r - g - b - back to r.
uint32_t Wheel(byte WheelPos) {
	WheelPos = 255 - WheelPos;
	if (WheelPos < 85) {
		return strip.Color(255 - WheelPos * 3, 0, WheelPos * 3);
	}
	if (WheelPos < 170) {
		WheelPos -= 85;
		return strip.Color(0, WheelPos * 3, 255 - WheelPos * 3);
	}
	WheelPos -= 170;
	return strip.Color(WheelPos * 3, 255 - WheelPos * 3, 0);
}

void dimDown(uint8_t wait, uint8_t red, uint8_t green, uint8_t blue, uint8_t white) {
	uint8_t ledCount = 3;
	int leds[] = {random(0,17), random(0,17), random(0,17)};
	for(uint8_t j = 0; j < ledCount; j++) {
		strip.setPixelColor(j, strip.Color(red, green, blue, white));
	}
	for(uint8_t i = 0; i < 255; i++) {
		strip.show();
		for(uint8_t j = 0; j < ledCount; j++) {
			strip.setPixelColor(j, strip.Color(red - ((red / 255) * i), green - ((green / 255) * i), blue - ((blue / 255) * i), white - ((white / 255) * i)));
		}
		delay(wait);
	}
	for(uint8_t j = 0; j < ledCount; j++) {
		strip.setPixelColor(j, strip.Color(0, 0, 0, 0));
	}
	strip.show();
}
