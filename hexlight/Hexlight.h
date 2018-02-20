#include <Adafruit_NeoPixel.h>
#include <ArduinoOTA.h>
#include "HexlightWifi.h"

#ifndef Hexlight_h
#define Hexlight_h
class Hexlight : public HexlightWifi {
	public:
		Adafruit_NeoPixel strip;
		Hexlight(uint8_t pin, uint8_t numLeds);
		void delay(unsigned long ms);
	private:
};
#endif
