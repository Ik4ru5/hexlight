#include <Adafruit_NeoPixel.h>
#include <ESP8266WiFi.h>
#include <ESP8266mDNS.h>
#include <ESP8266WebServer.h>
#include <WiFiUdp.h>
#include <WiFiClient.h>
#include <ArduinoOTA.h>




#define PIN 0
#define NUM_LEDS 126

const int accesspointmode = 1; // set to 0 to connect to an existing network or leave it set to 1 to use the esp8266 as an access point
const char ssid[] = "SK6812";
const char password[] = "";
const int channel = 6;
const int hidden = 0; // Set as 0 to broadcast AP's SSID or as 1 to hide SSID

int DelayLength=2000; //Length of time in ms to wait between sending lines from payload
IPAddress local_IP(192,168,1,1); //IP of the esp8266 server
IPAddress gateway(192,168,1,1);
IPAddress subnet(255,255,255,0);
ESP8266WebServer server(80);

const String HTML_CSS_STYLING = "<style>a,body{background-color: #000;color: #0f0;}</style>";
const String HTML_BACK_TO_INDEX = "<a href=\"/\"><- BACK TO INDEX</a><br><br>";



// Parameter 1 = number of pixels in strip
// Parameter 2 = Arduino pin number (most are valid)
// Parameter 3 = pixel type flags, add together as needed:
//   NEO_KHZ800  800 KHz bitstream (most NeoPixel products w/WS2812 LEDs)
//   NEO_KHZ400  400 KHz (classic 'v1' (not v2) FLORA pixels, WS2811 drivers)
//   NEO_GRB     Pixels are wired for GRB bitstream (most NeoPixel products)
//   NEO_RGB     Pixels are wired for RGB bitstream (v1 FLORA pixels, not v2)
//   NEO_RGBW    Pixels are wired for RGBW bitstream (NeoPixel RGBW products)
Adafruit_NeoPixel strip = Adafruit_NeoPixel(NUM_LEDS, PIN, NEO_GRBW + NEO_KHZ800);

// IMPORTANT: To reduce NeoPixel burnout risk, add 1000 uF capacitor across
// pixel power leads, add 300 - 500 Ohm resistor on first pixel's data input
// and minimize distance between Arduino and first pixel.  Avoid connecting
// on a live circuit...if you must, connect GND first.

void setup() {
  Serial.begin(9600);
  
  //pinMode(LED_BUILTIN, OUTPUT); 
  
// Determine if set to Access point mode
  if (accesspointmode == 1) {
    WiFi.softAPConfig(local_IP, gateway, subnet);
    WiFi.softAP(ssid, password, channel, hidden);
  }
  else if (accesspointmode != 1) {
    WiFi.config(local_IP, gateway, subnet);
    WiFi.begin(ssid, password);
  }

    // Port defaults to 8266
  // ArduinoOTA.setPort(8266);

  // Hostname defaults to esp8266-[ChipID]
  // ArduinoOTA.setHostname("myesp8266");

  // No authentication by default
  // ArduinoOTA.setPassword((const char *)"123");

  ArduinoOTA.onStart([]() {
    Serial.println("Start");
  });
  ArduinoOTA.onEnd([]() {
    Serial.println("\nEnd");
  });
  ArduinoOTA.onProgress([](unsigned int progress, unsigned int total) {
    Serial.printf("Progress: %u%%\r", (progress / (total / 100)));
  });
  ArduinoOTA.onError([](ota_error_t error) {
    Serial.printf("Error[%u]: ", error);
    if (error == OTA_AUTH_ERROR) Serial.println("Auth Failed");
    else if (error == OTA_BEGIN_ERROR) Serial.println("Begin Failed");
    else if (error == OTA_CONNECT_ERROR) Serial.println("Connect Failed");
    else if (error == OTA_RECEIVE_ERROR) Serial.println("Receive Failed");
    else if (error == OTA_END_ERROR) Serial.println("End Failed");
  });
  ArduinoOTA.begin();
  Serial.println("Ready");
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());

  strip.begin();
  strip.show();
}

void loop() {
  ArduinoOTA.handle();
  // Some example procedures showing how to display to the pixels:
  colorWipe(strip.Color(255, 0, 0), 50); // Red
  colorWipe(strip.Color(0, 255, 0), 50); // Green
  colorWipe(strip.Color(0, 0, 255), 50); // Blue
  colorWipe(strip.Color(0, 0, 0, 255), 50); // White RGBW
  // Send a theater pixel chase in...
  theaterChase(strip.Color(127, 127, 127), 50); // White
  theaterChase(strip.Color(127, 0, 0), 50); // Red
  theaterChase(strip.Color(0, 0, 127), 50); // Blue

  rainbow(20);
  rainbowCycle(random(1,50));
  //theaterChaseRainbow(random(5,30), random(1,10));
  //nerv();
}




void setSubmodule(uint16_t submodule, uint32_t color) {
  setSide((submodule * 2), color);
  setSide((submodule * 2) + 1, color);
}

void setSide(uint16_t s, uint32_t c) {
  uint16_t led = s * 3;
  strip.setPixelColor(led, c);
  strip.setPixelColor(led + 1, c);
  strip.setPixelColor(led + 2, c);
}

// Fill the dots one after the other with a color
void colorWipe(uint32_t c, uint8_t wait) {
  for(uint16_t i=0; i<strip.numPixels()/6; i++) {
    setSubmodule(i, c);
    strip.show();
    delay(wait);
  }
}

void nerv() {
  for(uint16_t i = 0; i < NUM_LEDS / 3; i++) {
    strip.setPixelColor(i, strip.Color(random(0, 255), random(0, 255), random(0, 255), random(0, 255)));
  }
  strip.show();
}

void rainbow(uint8_t wait) {
  uint16_t i, j;

  for(j=0; j<256; j++) {
    for(i=0; i<strip.numPixels() / 6; i++) {
      setSubmodule(i, Wheel((i+j) & 255));
    }
    strip.show();
    delay(wait);
  }
}

// Slightly different, this makes the rainbow equally distributed throughout
void rainbowCycle(uint8_t wait) {
  uint16_t i, j;

  for(j=0; j<256*5; j++) { // 5 cycles of all colors on wheel
    for(i=0; i< strip.numPixels()/6; i++) {
      setSubmodule(i, Wheel(((i * 256 / strip.numPixels()) + j) & 255));
    }
    strip.show();
    delay(wait);
  }
}

//Theatre-style crawling lights.
void theaterChase(uint32_t c, uint8_t wait) {
  for (int j=0; j<10; j++) {  //do 10 cycles of chasing
    for (int q=0; q < 3; q++) {
      for (uint16_t i=0; i < strip.numPixels()/6; i=i+3) {
        setSubmodule(i+q, c);    //turn every third pixel on
      }
      strip.show();

      delay(wait);

      for (uint16_t i=0; i < strip.numPixels()/6; i=i+3) {
        setSubmodule(i+q, 0);        //turn every third pixel off
      }
    }
  }
}

//Theatre-style crawling lights with rainbow effect
void theaterChaseRainbow(uint8_t wait, uint8_t steps) {
  for (int j=0; j < 256; j+=steps) {     // cycle all 256 colors in the wheel
    for (int q=0; q < 3; q++) {
      uint8_t s = random(3,10);
      for (uint16_t i=0; i < strip.numPixels()/6; i=i+s) {
        setSubmodule(i+q, Wheel( (i+j) % 255));    //turn every third pixel on
      }
      strip.show();

      delay(wait);

      for (uint16_t i=0; i < strip.numPixels()/6; i=i+s) {
        setSubmodule(i+q, 0);        //turn every third pixel off
      }
    }
  }
}

// Input a value 0 to 255 to get a color value.
// The colours are a transition r - g - b - back to r.
uint32_t Wheel(byte WheelPos) {
  WheelPos = 255 - WheelPos;
  if(WheelPos < 85) {
    return strip.Color(255 - WheelPos * 3, 0, WheelPos * 3);
  }
  if(WheelPos < 170) {
    WheelPos -= 85;
    return strip.Color(0, WheelPos * 3, 255 - WheelPos * 3);
  }
  WheelPos -= 170;
  return strip.Color(WheelPos * 3, 255 - WheelPos * 3, 0);
}
