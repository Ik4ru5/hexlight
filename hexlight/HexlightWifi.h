#include <ESP8266WiFi.h>
#include <ESP8266WiFiMulti.h>
#include "debuggable.h"

#ifndef HexlightWifi_h
#define HexlightWifi_h
class HexlightWifi : public Debuggable {
	public:
		HexlightWifi();
		void handleWifi();
		ESP8266WiFiMulti wifiMulti;
	private:

};
#endif