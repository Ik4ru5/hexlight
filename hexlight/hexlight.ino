#include "Hexlight.h"


#define PIN 0
#define NUM_LEDS 18


Hexlight *hex;

void setup() {
	hex = new Hexlight(PIN, NUM_LEDS, "ota");
	hex->debug("SETUP");
}

void loop() {
	hex->handle();
	hex->handlePatterns();
}



void dimDown(uint8_t wait, uint8_t red, uint8_t green, uint8_t blue, uint8_t white) {
	uint8_t ledCount = 3;
	int leds[] = {random(0,17), random(0,17), random(0,17)};
	for(uint8_t j = 0; j < ledCount; j++) {
		hex->strip.setPixelColor(j, hex->strip.Color(red, green, blue, white));
	}
	for(uint8_t i = 0; i < 255; i++) {
		hex->strip.show();
		for(uint8_t j = 0; j < ledCount; j++) {
			hex->strip.setPixelColor(j, hex->strip.Color(red - ((red / 255) * i), green - ((green / 255) * i), blue - ((blue / 255) * i), white - ((white / 255) * i)));
		}
		delay(wait);
	}
	for(uint8_t j = 0; j < ledCount; j++) {
		hex->strip.setPixelColor(j, hex->strip.Color(0, 0, 0, 0));
	}
	hex->strip.show();
}
