// rainbow only with old code

#include <Adafruit_NeoPixel.h>

#define PIN 5
/*------------------------------------------------------------*/
double first = 0;
double second = 0;
double initalValue = 50;
double tempValue = 1;
double value = initalValue;
double valueIncre = -1;
boolean isBlink = false;

Adafruit_NeoPixel strip = Adafruit_NeoPixel(69, PIN, NEO_GRB + NEO_KHZ800);
/*------------------------------------------------------------*/
int analogPin = A5; // pin for msgeq7 input
int strobePin = 3; // msgeq7 strobe pin for cycling through channels 
int resetPin = 4; // reset pin of the msgeq7
int numberOfChannels = 7; // number of channels in msgeq7
int spectrumValue[7]; // array to store the 7 values of the 7 channels

int resetDelay = 80; // delay of resetting MSGEQ7 in microseconds
int strobeDelay_USec = 15; // delay to settle input recording in microseconds

int breakPoint = 500; // lower cut off point of output number
int counter = 0; // counter of averaging breakPoint
int sumValues = 0; // current sum of values
int minValue =  130;

float sumValueChange = 1.1; // multiply average value by this
/*------------------------------------------------------------*/
int switcherPin = 13; // switch to turn off sound reactive
/*------------------------------------------------------------*/

void setup() {
  delay(3000); // safe bootup

  Serial.begin(9600);

  pinMode(switcherPin, INPUT);
    
  strip.begin();
  strip.show(); // Initialize all pixels to 'off'
  
  // turn on pins for msgeq7
  pinMode(analogPin, INPUT);
  pinMode(strobePin, OUTPUT);
  pinMode(resetPin, OUTPUT);
  
  digitalWrite(resetPin, LOW);
  digitalWrite(strobePin, HIGH);
}
/*------------------------------------------------------------*/
void loop() {
  rainbowCycle();
}

/*------------------------------------------------------------*/
void rainbowCycle() {  
  for(int j=0; j<256*5; j++) { // 5 cycles of all colors on wheel    
    for(int i=0;i<strip.numPixels();i++) {
        value = 1;
        strip.setPixelColor(i, Wheel(((i * 256 / strip.numPixels()) + j) & 255));
    }

    strip.show();
  }
}
  
/*------------------------------------------------------------*/
uint32_t Wheel(byte WheelPos) {
  if(WheelPos < 85) {
    first = (WheelPos * 3) / value;
    second = (255 - WheelPos * 3) / value;
    return strip.Color(first, second, 0);
  } else if(WheelPos < 170) {
     WheelPos -= 85;
     first = (WheelPos * 3) / value;
     second = (255 - WheelPos * 3) / value;
     return strip.Color(second, 0, first);
  } else {
     WheelPos -= 170;
     first = (WheelPos * 3) / value;
     second = (255 - WheelPos * 3) / value;
     return strip.Color(0, first, second);
  }
}

/*------------------------------------------------------------*/
void msgeq7(){
    digitalWrite(resetPin, HIGH);
    delayMicroseconds(resetDelay);
    digitalWrite(resetPin, LOW);
    
     for(int i=0;i<numberOfChannels;i++){
       // start reading channel by changing strobe to low
       digitalWrite(strobePin,LOW);
       // allows input to settle to get accurate reading
       delayMicroseconds(strobeDelay_USec);
       // read value of current pin from msgeq7
       spectrumValue[i]=analogRead(analogPin);
       // print out value to serial monitor
      
       sumValues += spectrumValue[i];
         
       if(i>numberOfChannels-2){
         // take average of values and reset counter/sum values
         breakPoint = (sumValues/numberOfChannels)*sumValueChange;
         sumValues = 0;
       } // else set average of break point and reset value
       // strobe pin high the low to go to next channel on msgeq7  
       digitalWrite(strobePin, HIGH);
     } // for each input do this
     
     //for(int i=0;i<numberOfChannels;i++){Serial.print(spectrumValue[i]);Serial.print(" ");}
     //Serial.println();
     //Serial.println(breakPoint);
}
