#include "HLC.h"


void HLC::receivePacket() {
  packetSize = Udp.parsePacket();
  if (packetSize) {
    Serial.printf("Received %d bytes from %s, port %d\n", packetSize, Udp.remoteIP().toString().c_str(), Udp.remotePort());
    int len = Udp.read(packet, 255);
    if (len > 0) {
      packet[len] = 0;
    }
  }
}
