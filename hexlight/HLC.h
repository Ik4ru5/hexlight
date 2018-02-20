#ifndef HLC_h
#define HLC_h

#include<Arduino.h>
#include <WiFiUdp.h>

#define PATTERN_CONTROL_CHAR 'p'
#define SINGEL_CONTROL_CHAR '#'
#define MULTI_CONTROL_CHAR '+'

class HLC {
  public:
    void receivePacket();
    
  private:
    char packet[255]; // check max payload length
    uint16_t packetSize;
    WiFiUDP Udp;
};
#endif
