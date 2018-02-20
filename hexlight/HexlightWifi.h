#include <ESP8266WiFi.h>
#include <ESP8266WiFiMulti.h>
#include <ESP8266mDNS.h>
#include <WiFiUdp.h>
#include "debuggable.h"

#ifndef HexlightWifi_h
#define HexlightWifi_h
class HexlightWifi : public Debuggable {
	public:
		HexlightWifi();
		uint8_t handle();
		ESP8266WiFiMulti wifiMulti;
		WiFiUDP Udp;
	private:

};
#endif