#include <Adafruit_NeoPixel.h>
#include "WS2812_Definitions.h"

#define PIN 4
#define LED_COUNT 7
Adafruit_NeoPixel leds = Adafruit_NeoPixel(LED_COUNT, PIN, NEO_GRB + NEO_KHZ800);

int x= 10;
int y = 0;

void setup()
{
  leds.begin();  // Call this to start up the LED strip.
}

void loop()
{
  leds.setPixelColor(0, 0x0000FF);
  leds.setPixelColor(1, 0xF0FFFF);
  leds.setPixelColor(2, 0x800080);
  leds.setPixelColor(3, 0x8B0000);
  leds.setPixelColor(4, 0xA52A2A);
  leds.setPixelColor(5, 0xFFC0CB);
  leds.setPixelColor(6, 0xFFFF00);
  delay(10);
  x =x+10;
  leds.show();
  leds.setBrightness(x);
  if(x >240){
    x=10;
   delay(500); 
  }
}


