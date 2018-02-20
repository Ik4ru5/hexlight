#include "HexlightWifi.h"

HexlightWifi::HexlightWifi() {
	this->debug("HexlightWifi");
	WiFi.mode(WIFI_AP_STA);
	// TODO: Read Hotspots from Array or something
	wifiMulti.addAP("NA NA NA NA NA NA BATLAN!!", "niemandmussbullesein");
	wifiMulti.addAP("Fuba", "moppelkotze");
	wifiMulti.addAP("Fablab Karlsruhe", "foobar42");
	
	if(wifiMulti.run() == WL_CONNECTED) {
		Serial.println("WiFi connected");
		Serial.println("IP address: ");
		Serial.println(WiFi.localIP());
	}
	
	// TODO: Create backup Wifi station
}

uint8_t HexlightWifi::handle() {
	uint8_t status = wifiMulti.run();
	if(status != WL_CONNECTED) {
		Serial.println("WiFi not connected!");
		delay(1000);
	}
	return status;
}



