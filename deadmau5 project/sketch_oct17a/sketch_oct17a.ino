int numLEDS = 70;
int hue = 0;
int eightBit = 255;

void setup() {
  // put your setup code here, to run once:

}

void loop() {

  for(int i=0;i<numLED;i++){
  
    hueVal = i + hue;
  
    if (hueVal >= eightBit) {
      hueVal = hueVal - eightBit;
    }
  
    led[i].setHue(hueVal);
  }

  led.show()

}
