#include <WiFiUdp.h>
#include <ESP8266WiFiMulti.h>
#include <NTPClient.h>
#include <Time.h>
#include "debuggable.h"

#ifndef HexlightWifi_h
#define HexlightWifi_h

class HexlightWifi : public Debuggable {
	public:
		HexlightWifi();
		~HexlightWifi();
		void handleWifi();
		void parsePacket();
		void resetPacket();
		
		ESP8266WiFiMulti wifiMulti;
		WiFiUDP Udp;
		bool newPacket;
		bool csp;
		int pattern;
		unsigned int localUdpPort = 4210;  // local port to listen on
		char incomingPacket[255];  // buffer for incoming packets
		uint8_t red = 0;
		uint8_t green = 0;
		uint8_t blue = 0;
		uint8_t white = 0;
};
#endif
