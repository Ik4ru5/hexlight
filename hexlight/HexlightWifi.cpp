#include "HexlightWifi.h"

HexlightWifi::HexlightWifi() : Debuggable() {
	// WiFi.mode(WIFI_AP_STA);
	// TODO: Read Hotspots from Array or something
	
	this->wifiMulti.addAP("WLAN-592980", "8081513424484664");
	this->wifiMulti.addAP("NA NA NA NA NA NA BATLAN!!", "niemandmussbullesein");
	this->wifiMulti.addAP("net6", "moppelkotze");
	this->wifiMulti.addAP("Fuba", "moppelkotze");
	this->wifiMulti.addAP("Fablab Karlsruhe", "foobar42");

	if(this->wifiMulti.run() == WL_CONNECTED) {
		Serial.println("WiFi connected");
		Serial.println("IP address: ");
		Serial.println(WiFi.localIP());
	}
	
	// TODO: Create backup Wifi station
}

void HexlightWifi::handleWifi() {
	uint8_t status = this->wifiMulti.run();
	if(status != WL_CONNECTED) {
		Serial.println("WiFi not connected!");
		delay(1000);
	}
	//else {
	
	//}
}
