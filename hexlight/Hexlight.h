#include <Adafruit_NeoPixel.h>
#include <ArduinoOTA.h>
#include <WiFiUdp.h>
#include <time.h>
#include "HexlightWifi.h"
extern "C" {
#include "user_interface.h"
}

#define PORT 2323	

#ifndef Hexlight_h
#define Hexlight_h
class Hexlight : public HexlightWifi {
	public:
		Adafruit_NeoPixel strip;
		
		Hexlight(uint8_t pin, uint8_t numLeds, char* OTApass);
		
		void hexDelay(unsigned long ms);
		uint32_t handle();
		void handlePatterns();
		void candle();
		void flash(uint32_t c, uint8_t t, uint8_t wait);
		void colorSet(uint32_t c);
		void rainbowCycle(uint8_t wait);
		void colorWipe(uint32_t c, uint8_t wait);
		uint32_t Wheel(byte WheelPos);
	private:
		void initStrip(uint8_t pin, uint8_t numLeds);
		void initOTA(char* OTApass);
		void initOnError();
		void initOnStart();
		void initOnEnd();
		void initOnProgress();
		
		bool checkNewPacket();
		void parsePacket();
		void parseColor(char* incomingPacket, int i);
		
		WiFiUDP udp;
		bool packetInQueue;
		uint16_t pattern;//maybe enum?
		// TODO: Determin max size
		char incomingPacket[255]; // propably too much
		uint8_t red = 0;
		uint8_t green = 0;
		uint8_t blue = 0;
		uint8_t white = 0;
};
#endif