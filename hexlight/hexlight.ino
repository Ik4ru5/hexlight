#include "Hexlight.h"

#define PIN 0
#define NUM_LEDS 18

Hexlight *hex;
Adafruit_NeoPixel strip = Adafruit_NeoPixel(NUM_LEDS, PIN, NEO_GRBW + NEO_KHZ800);

void setup() {
	Serial.begin(115200);
	
	strip.begin();
	strip.show();
	
	hex = new Hexlight(strip);
	delay(10);
	hex->colorSet(strip.Color(0, 0, 0, 255)); // init with white
}

/* ideas:
bidirectional handshake to figure out which hexlight has rebootet
	every X frames a heartbeat from the controller
	every Y frames a heartbeat from the esp
	heartbeat contains:
		current step
		...
*/

void loop() {
	hex->handle();
}
