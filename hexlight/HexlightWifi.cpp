#include "HexlightWifi.h"

HexlightWifi::HexlightWifi() : Debuggable() {
	csp = false;
	WiFi.mode(WIFI_STA);
	// TODO: Read Hotspots from Array or something

	//this->wifiMulti.addAP("WLAN-592980", "8081513424484664");
	this->wifiMulti.addAP("NA NA NA NA NA NA BATLAN!!", "niemandmussbullesein");
	this->wifiMulti.addAP("Foobar", "donotuse");
	this->wifiMulti.addAP("net6", "moppelkotze");
	this->wifiMulti.addAP("Fuba", "moppelkotze");
	this->wifiMulti.addAP("Fablab Karlsruhe", "foobar42");
	this->wifiMulti.addAP("darmstadt.ccc.de", "cleanupwhenyouleave");
	//this->wifiMulti.addAP("psylight", "gibt'snicht!");
	//this->wifiMulti.addAP("Nukleares Forschungsnetz", "bobderbaumeister");
	//this->wifiMulti.addAP("darmstadt.freifunk.net", "");

	if (this->wifiMulti.run() == WL_CONNECTED) {
		Serial.println("WiFi connected");
		Serial.println("IP address: ");
		Serial.println(WiFi.localIP());
	}
	this->Udp.begin(this->localUdpPort);
	Serial.printf("Now listening at IP %s, Udp port %d\n", WiFi.localIP().toString().c_str(), this->localUdpPort);

	// TODO: Create backup Wifi station
	Serial.println("HexlightWifi Setup");
}

HexlightWifi::~HexlightWifi() {
	Serial.println("HexlightWifi shutting down!");
}

void HexlightWifi::handleWifi() {
	uint8_t status = this->wifiMulti.run();

	//if (status != WL_CONNECTED) {
		//Serial.println("WiFi not connected!");
		//delay(500);
	//}

	if (status == WL_CONNECTED && csp == false) {
		Serial.println("WiFi connected");
		Serial.println("IP address: ");
		Serial.println(WiFi.localIP());
		csp = true;
	}
	this->parsePacket();
}

void HexlightWifi::parsePacket() {
	int packetSize = this->Udp.parsePacket();
	if (packetSize) {
		// receive incoming Udp packets
		Serial.printf("Received %d bytes from %s, port %d\n", packetSize, this->Udp.remoteIP().toString().c_str(), this->Udp.remotePort());
		int len = this->Udp.read(this->incomingPacket, 255);
		if (len > 0) {
			this->incomingPacket[len] = 0;
			this->newPacket = true;
			if (this->Udp.localPort() == 123) {
				this->Udp.beginPacket(this->Udp.remoteIP(), this->Udp.remotePort());
				this->Udp.write("NTP");
				this->Udp.endPacket();
			}
		}
		Serial.printf("Udp packet contents: %s\n", incomingPacket);

		// send back a reply, to the IP address and port we got the packet from
		this->Udp.beginPacket(this->Udp.remoteIP(), this->Udp.remotePort());
		this->Udp.write("ACK");
		this->Udp.endPacket();
	}
}

void HexlightWifi::resetPacket() {
	this->newPacket = false;
	for (int i = 0; i < 255; i++) {
		this->incomingPacket[i] = 0;
	}
}
