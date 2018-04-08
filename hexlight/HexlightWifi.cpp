#include "HexlightWifi.h"

HexlightWifi::HexlightWifi() : Debuggable() {
	csp = false;
	WiFi.mode(WIFI_STA);
	// TODO: Read Hotspots from Array or something
	
	//this->wifiMulti.addAP("WLAN-592980", "8081513424484664");
	this->wifiMulti.addAP("NA NA NA NA NA NA BATLAN!!", "niemandmussbullesein");
	//this->wifiMulti.addAP("Foobar", "donotuse");
	//this->wifiMulti.addAP("net6", "moppelkotze");
	this->wifiMulti.addAP("Fuba", "moppelkotze");
	this->wifiMulti.addAP("Fablab Karlsruhe", "foobar42");
	delay(10);


	if(this->wifiMulti.run() == WL_CONNECTED) {
		Serial.println("WiFi connected");
		Serial.println("IP address: ");
		Serial.println(WiFi.localIP());
	}
	this->Udp.begin(this->localUdpPort);
	Serial.printf("Now listening at IP %s, UDP port %d\n", WiFi.localIP().toString().c_str(), this->localUdpPort);
	 
	// TODO: Create backup Wifi station
	this->debug("HexlightWifi Setup");
}

HexlightWifi::~HexlightWifi() {
	Serial.println("HexlightWifi shutting down!");
}

void HexlightWifi::handleWifi() {
	uint8_t status = this->wifiMulti.run();
	if(status != WL_CONNECTED) {
		Serial.println("WiFi not connected!");
		delay(1000);
	}
	
	if(this->wifiMulti.run() == WL_CONNECTED && csp == false) {
		Serial.println("WiFi connected");
		Serial.println("IP address: ");
		Serial.println(WiFi.localIP());
		csp = true;
	}
}

void HexlightWifi::parsePacket() {
		int packetSize = this->Udp.parsePacket();
	if (packetSize)
	{
		// receive incoming UDP packets
		Serial.printf("Received %d bytes from %s, port %d\n", packetSize, this->Udp.remoteIP().toString().c_str(), this->Udp.remotePort());
		int len = this->Udp.read(this->incomingPacket, 255);
		if (len > 0)
		{
			this->incomingPacket[len] = 0;
		}
		Serial.printf("UDP packet contents: %s\n", incomingPacket);
		
		// send back a reply, to the IP address and port we got the packet from
		this->Udp.beginPacket(this->Udp.remoteIP(), this->Udp.remotePort());
		this->Udp.write("Hi there! Got the message :-)");
		this->Udp.endPacket();
	}
}

void Hexlight::parseColor(int i) {
/*
	switch(this->incomingPacket[i]) {
		case 98: // b
		case 66: // B
			this->blue = this->incomingPacket[i+1];
			break;
		case 82:	// R
		case 114: // r
			this->red = this->incomingPacket[i+1];
			break;
		case 103: // g
		case 71:	// G
			this->green = this->incomingPacket[i+1];
			break;
		case 119: // w
		case 87:	// W
			this->white = this->incomingPacket[i+1];
			break;
		case 120: //x
		case 88:	//X
			this->red = 0;
			this->green = 0;
			this->blue = 0;
			this->white = 0;
			break;
	}
*/
}
