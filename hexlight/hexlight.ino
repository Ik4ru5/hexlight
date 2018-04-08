#include "Hexlight.h"

#define PIN 0
#define NUM_LEDS 18

Hexlight hex = Hexlight(PIN, NUM_LEDS, "ota");
bool output = true;

void setup() {
	Serial.begin(115200);
	
	delay(10);

	
	Serial.printf("Size of hex: %i", sizeof(hex));
	hex.debug("SETUP");
}

void loop() {
	hex.handle();
	hex.pattern = 3;
	hex.doStep();
}