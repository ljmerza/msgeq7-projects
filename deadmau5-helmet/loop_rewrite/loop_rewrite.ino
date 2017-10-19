#include <Adafruit_NeoPixel.h>

#define PIN 5
/*------------------------------------------------------------*/
const int numberOfLeds = 69; // the number of LEDS
Adafruit_NeoPixel strip = Adafruit_NeoPixel(numberOfLeds, PIN, NEO_GRB + NEO_KHZ800);
/*------------------------------------------------------------*/
const int analogPin = A5; // pin for msgeq7 input
const int strobePin = 3; // msgeq7 strobe pin for cycling through channels 
const int resetPin = 4; // reset pin of the msgeq7
const int switcherPin = 13; // switch pin

const int numberOfChannels = 7; // number of channels in msgeq7
int spectrumValues[numberOfChannels]; // array to store the 7 values of the 7 channels
int spectrumValue; // place to save the current spectrum value being compared on

const int resetDelay = 80; // delay of resetting MSGEQ7 in microseconds
const int strobeDelay_USec = 15; // delay to settle input recording in microseconds

int breakPoint = 0; // lower cut off point of output number ( average * sumValueMultiplier )
const int minValue =  130; // min value MSGEQ7 channels are allowed to be to trigger light

const float sumValueMultiplier = 1.1; // multiply average value by this 

// create LED matrix
const int numberOfLedsInGroup = 14;
int ledPositions[numberOfChannels][numberOfLedsInGroup] = {
  {0,7,14,20,25,32,35,37,41,43,45,50,55,62},
  {1,8,15,21,26,33,46,51,56,63,99,99,99,99},
  {2,9,16,22,27,34,47,52,57,64,99,99,99,99},
  {3,10,17,23,28,53,58,65,99,99,99,99,99,99},
  {4,11,18,24,29,36,38,42,44,54,59,66,99,99},
  {5,12,19,30,39,48,60,67,99,99,99,99,99,99},
  {6,13,31,40,49,61,68,99,99,99,99,99,99,99}
};

int ledGroup[numberOfLedsInGroup]; // a place to save the currently worked on LED group
int brightnessDivider; // how much to divide the brightness by
int originalDividerValue; // holds the original divider value while changing the brightness

/*------------------------------------------------------------*/
void setup() {
  Serial.begin(9600);
    
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
  
  // for all 5 cycles of all colors on wheel
  for(int i=0; i<256*5; i++) {
    
    // get all MSGEQ7 values
    //msgeq7();

    // for each LED in a strip
    for(int j=0;j<strip.numPixels();j++) {
      // for each MSGEQ7 channel
      for(int k=0;k<numberOfChannels;k++){

        // get current LED group's specturm value
        spectrumValue = spectrumValues[k];

        // for each LED in the LED group
        for(int l=0; l<numberOfLedsInGroup;l++){

          // if current LED on the strip matches current LED in current channel group
          // and the specturm value for the LED group is lower than the breakpoint
          // or the spectrum value is lower than the min value allowed to change LEDs
          if(ledPositions[k][l] == i && (spectrumValue<breakPoint || spectrumValue<minValue)){
            // save the original brightness divider value and set it to 1
            originalDividerValue = brightnessDivider;
            brightnessDivider = 1;
          }
        }
      }

      // set LED color/brightness
      strip.setPixelColor( j, setLedColor(((i*256/strip.numPixels())+j)&255) );
    }

    // if we changed the brightness divider then reset it here
    if(brightnessDivider == 1){
      brightnessDivider = originalDividerValue;
    }

    // send all new LED brightness values to arduino
    strip.show();
  }
}
  
uint32_t setLedColor(byte wheelPos) {
  double first = 0; // first color calculation
  double second = 0; // second color calculation

  if(wheelPos < 85) {
    first = (wheelPos * 3) / brightnessDivider;
    second = (255 - wheelPos * 3) / brightnessDivider;
    return strip.Color(first, second, 0);

  } else if(wheelPos < 170) {
    wheelPos -= 85;
    first = (wheelPos * 3) / brightnessDivider;
    second = (255 - wheelPos * 3) / brightnessDivider;
    return strip.Color(second, 0, first);

  } else {
    wheelPos -= 170;
    first = (wheelPos * 3) / brightnessDivider;
    second = (255 - wheelPos * 3) / brightnessDivider;
    return strip.Color(0, first, second);
  }
}

/*------------------------------------------------------------*/
void msgeq7(){
    digitalWrite(resetPin, HIGH);
    delayMicroseconds(resetDelay);
    digitalWrite(resetPin, LOW);

    // reset sum of values
    int sumValues = 0;  
    
    for(int i=0;i<numberOfChannels;i++){
      // start reading channel by changing strobe to low
      digitalWrite(strobePin,LOW);
      // allows input to settle to get accurate reading
      delayMicroseconds(strobeDelay_USec);
      // read value of current pin from msgeq7
      spectrumValues[i]=analogRead(analogPin);
      // sum value
      sumValues += spectrumValues[i];
         
      // strobe pin high the low to go to next channel on msgeq7  
      digitalWrite(strobePin, HIGH);
      
      // if we summed all values then get avergage and return 
      if(i>numberOfChannels-2){
        // take average of values and reset counter/sum values
        breakPoint = (sumValues/numberOfChannels)*sumValueMultiplier;
      }
    }
     
    // for debugging - print is computationally heavy so to save time only do one big print
    //Serial.print(spectrumValues[0] + ' ' + spectrumValues[1] + ' ' + spectrumValues[2] + ' ' + spectrumValues[3] + ' ' + spectrumValues[4] + ' ' + spectrumValues[5] + ' ' + spectrumValues[6] + '\n' + breakPoint);

}








