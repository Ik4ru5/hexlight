#include <Adafruit_NeoPixel.h>
#include <ArduinoOTA.h>

#include "HexlightWifi.h"


#ifndef Hexlight_h
#define Hexlight_h
class Hexlight : public HexlightWifi {
	public:
		Adafruit_NeoPixel strip;
		uint8_t pattern;
		Hexlight();
		Hexlight(uint8_t pin, uint8_t numLeds, char* OTApass);
		~Hexlight();
		
		void hexDelay(unsigned long ms);
		void handle();
		void doStep();
		

		void hexParsePacket();
		void resetPacket();
		
		// Patterns
		void candle();
		void flash(uint32_t c, uint8_t t, uint8_t wait);
		void colorSet(uint32_t c);
		void rainbowCycle(uint8_t wait);
		void colorWipe(uint32_t c, uint8_t wait);
		
		//Color helper
		uint32_t Wheel(byte WheelPos);
		unsigned long lastStep;
		uint32_t step;
		
		// TODO: Determin max size
		//char incomingPacket[255]; // propably too much
		
	private:
		void initStrip(uint8_t pin, uint8_t numLeds);
		
		// OTA
		void initOTA(char* OTApass);
		void initOnError();
		void initOnStart();
		void initOnEnd();
		void initOnProgress();
		
		
		uint8_t red = 0;
		uint8_t green = 0;
		uint8_t blue = 0;
		uint8_t white = 0;
};
#endif