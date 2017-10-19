#include <FastLED.h>

#define LED_PIN     5
#define NUM_LEDS    70
#define BRIGHTNESS  64
#define LED_TYPE    WS2811
#define COLOR_ORDER GRB
CRGB leds[NUM_LEDS];

#define UPDATES_PER_SECOND 100

CRGBPalette16 currentPalette;
TBlendType    currentBlending;

extern CRGBPalette16 myRedWhiteBluePalette;
extern const TProgmemPalette16 myRedWhiteBluePalette_p PROGMEM;

/*------------------------------------------------------------*/
int analogPin = A5; // pin for msgeq7 input
int strobePin = 3; // msgeq7 strobe pin for cycling through channels 
int resetPin = 4; // reset pin of the msgeq7
int numberOfChannels = 7; // number of channels in msgeq7
int spectrumValue[7]; // array to store the 7 values of the 7 channels

int resetDelay = 80; // delay of resetting MSGEQ7 in microseconds
int strobeDelay_USec = 40; // delay to settle input recording in microseconds

int breakPoint = 500; // lower cut off point of output number
int counter = 0; // counter of averaging breakPoint
int sumValues = 0; // current sum of values
int minValue =  130;
int sumValueChange = 1;

int ledBrightness[7];
/*------------------------------------------------------------*/
int switcherPin = 10; // switch to turn off sound reactance

void setup() {
    delay( 3000 ); // power-up safety delay
    FastLED.addLeds<LED_TYPE, LED_PIN, COLOR_ORDER>(leds, NUM_LEDS).setCorrection(TypicalLEDStrip);
    FastLED.setBrightness(BRIGHTNESS);
    
    currentPalette = RainbowColors_p;
    currentBlending = LINEARBLEND;

    // turn on pins for msgeq7
    pinMode(analogPin, INPUT);
    pinMode(strobePin, OUTPUT);
    pinMode(resetPin, OUTPUT);
    
    digitalWrite(resetPin, LOW);
    digitalWrite(strobePin, HIGH);
}

void loop() {
    static uint8_t startIndex = 0;
    startIndex = startIndex + 1; /* motion speed */
    
    FillLEDsFromPaletteColors( startIndex);
    
    FastLED.show();
    FastLED.delay(1000 / UPDATES_PER_SECOND);

    Serial.begin(9600);
}

void FillLEDsFromPaletteColors( uint8_t colorIndex)
{
  int brightness = 255;
    
  for( int i = 0; i < NUM_LEDS; i++) {
    msgeq7();
    brightness = soundBrightness(i);
    //Serial.println(brightness);
      
    leds[i] = ColorFromPalette( currentPalette, colorIndex, brightness, currentBlending);
    colorIndex += 1;
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
     
     for(int i=0;i<numberOfChannels;i++){Serial.print(spectrumValue[i]);Serial.print(" ");}
     Serial.println(breakPoint);
}


int soundBrightness(int i){
  if(i==0 | i==7 | i==14  | i==20 | i==25 | i==32 | i==35 | i==37 | i==41 | i==43 | i==45 | i==50 | i==55 | i==62){
    return map(spectrumValue[0], 0, 500, 0, 255);
  }
  else if(i==1 | i==8 | i==15 | i==21 | i==26 | i==33 | i==46 | i==51 | i==56 | i==63){
    return map(spectrumValue[1], 0, 500, 0, 255);
  }
  else if(i==2 | i==9 | i==16 | i==22 | i==27 | i==34 | i==47 | i==52 | i==57 | i==64){
    return map(spectrumValue[2], 0, 500, 0, 255);
  }   
  else if(i==3 | i==10 | i==17 | i==23 | i==28 | i==53 | i==58 | i==65){
    return map(spectrumValue[3], 0, 500, 0, 255);
  }
  else if(i==4 | i==11 | i==18 | i==24 | i==29 | i==36 | i==38 | i==42 | i==44 | i==54 | i==59 | i==66){
    return map(spectrumValue[4], 0, 500, 0, 255);
  }
  else if(i==5 | i==12 | i==19 | i==30 | i==39 | i==48 | i==60 | i==67){
    return map(spectrumValue[5], 0, 500, 0, 255);
  }
   else if(i==6 | i==13 | i==31 | i==40 | i==49 | i==61 | i==68){
    return map(spectrumValue[6], 0, 500, 0, 255);
  }
  else{
    return 0;
  }
}

