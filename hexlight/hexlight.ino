#include "Hexlight.h"
#include <ESP8266WiFi.h>
#include <WiFiUdp.h>

#define PIN 0
#define NUM_LEDS 18

Hexlight *hex = new Hexlight(PIN, NUM_LEDS, "ota");;

void setup() {
	hex->debug("SETUP");
}

void loop() {
	hex->handle();
	hex->doStep();
}
