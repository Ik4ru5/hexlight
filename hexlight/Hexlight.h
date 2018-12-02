#include <Adafruit_NeoPixel.h>
#include <ArduinoOTA.h>

#include "HexlightWifi.h"

#ifndef Hexlight_h
#define Hexlight_h
class Hexlight : public HexlightWifi {
	public:
		Hexlight(Adafruit_NeoPixel& strip);
		~Hexlight();
		
		bool delayHandle();
		
		void hexDelay(unsigned long ms);
		void handle();
		void handlePatterns();
		void readPacket();
		void setPattern(uint8_t pattern);
		void parseColor();
		uint32_t parseTime(uint8_t pos);
		
		// Patterns
		void candle();
		void demo();
		void theaterChaseRainbow(uint8_t wait, uint8_t steps);
		void theaterChase(uint32_t c, uint8_t wait);
		void flash(uint32_t c, uint8_t wait);
		void colorSet(uint32_t c);
		void rainbowCycle(uint8_t wait);
		void rainbow(uint8_t wait);
		void singleLedRainbow(uint8_t wait);
		void colorWipe(uint32_t c, uint8_t wait);
		void handlePattern();
		
		uint32_t wheel(byte WheelPos);
		
		Adafruit_NeoPixel strip;
		int step = 0;
		int demoStep = 0;
		int8_t coulum = 0;
		int8_t row = 0;
		uint8_t pattern = 0;
		uint32_t color = 0;
		uint32_t time = 0;
		uint8_t brightness = 0;
		
	private:
		void initStrip(uint8_t pin, uint8_t numLeds);
		
		// OTA
		void initOTA(char* OTApass);
		void initOnError();
		void initOnStart();
		void initOnEnd();
		void initOnProgress();
};
#endif