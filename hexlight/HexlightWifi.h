//#include <ESP8266WiFi.h>	
#include <WiFiUdp.h>
#include <ESP8266WiFiMulti.h>
#include "debuggable.h"

#ifndef HexlightWifi_h
#define HexlightWifi_h
class HexlightWifi : public Debuggable {
	public:
		HexlightWifi();
		~HexlightWifi();
		void handleWifi();
		void parsePacket();
		void parseColor(int i);
		void readPcket();
		WiFiUDP Udp;
		unsigned int localUdpPort = 2323;  // local port to listen on
		char incomingPacket[255];  // buffer for incoming packets
		ESP8266WiFiMulti wifiMulti;
		bool csp;
	private:

};
#endif