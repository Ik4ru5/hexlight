#include "Hexlight.h"
#include <ESP8266WiFi.h>
#include <WiFiUdp.h>

extern "C" {
#include <cont.h>
  extern cont_t g_cont;
}

#define PIN 0
#define NUM_LEDS 18

Hexlight hex;

void setup() {
	Serial.begin(115200);
	
	delay(10);
	/*Serial.printf("PRE: unmodified stack   = %4d\n", cont_get_free_stack(&g_cont));
	register uint32_t *sp asm("a1");
	Serial.printf("PRE: current free stack = %4d\n", 4 * (sp - g_cont.stack));

	Hexlight *tmp = new Hexlight(PIN, NUM_LEDS, "ota");
	Serial.printf("Size of tmp: %i", sizeof(tmp));
	hex = *tmp;
	delete tmp;
		
	hex.debug("SETUP");
	
	Serial.printf("POST: unmodified stack   = %4d\n", cont_get_free_stack(&g_cont));
	// *sp asm("a1");
	Serial.printf("POST: current free stack = %4d\n", 4 * (sp - g_cont.stack));*/
}

void loop() {
	if(millis() % 2000 < 5) {
		Serial.printf("LOOP: unmodified stack   = %4d\n", cont_get_free_stack(&g_cont));
		register uint32_t *sp asm("a1");
		Serial.printf("LOOP: current free stack = %4d\n", 4 * (sp - g_cont.stack));
	}

	hex.handle();
	hex.doStep();
}