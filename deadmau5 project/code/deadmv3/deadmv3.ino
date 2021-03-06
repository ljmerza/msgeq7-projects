// hsv values only

#include <FastLED.h>

#define LED_PIN     5
#define NUM_LEDS    70
#define BRIGHTNESS  64
#define LED_TYPE    WS2811
CRGB leds[NUM_LEDS];

#define UPDATES_PER_SECOND 100

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





void setup() {
    delay(3000); // power-up safety delay

    FastLED.addLeds<LED_TYPE, LED_PIN>(leds, NUM_LEDS).setCorrection( TypicalLEDStrip );
    FastLED.setBrightness(BRIGHTNESS);

    Serial.begin(9600);
}


void loop() {
	static uint8_t startIndex = 0;
  startIndex = startIndex + 1; /* change for rainbow speed */

	rainbow(startIndex);
	FastLED.show();

  FastLED.delay(1000 / UPDATES_PER_SECOND);
}


void rainbow(uint8_t colorIndex){
	for( int i = 0; i < NUM_LEDS; i++) {

		leds[i] = CRGB( colorIndex, 128, 0);
		colorIndex += 3;

		msgeq7GetValues();
		msgeq7LedAdjust(i);
	}
}

/*
 * change HSV brightness based off msgeq7 input values
*/
void msgeq7LedAdjust(uint8_t i){
	if( (i==0 | i==7 | i==14  | i==20 | i==25 | i==32 | i==35 | i==37 | i==41 | i==43 | i==45 | i==50 | i==55 | i==62) & (spectrumValue[0]<breakPoint | minValue>breakPoint) ){
		leds[i] -= CRGB( 0, 0, 20 );
    }
    else if( (i==1 | i==8 | i==15 | i==21 | i==26 | i==33 | i==46 | i==51 | i==56 | i==63) & (spectrumValue[1]<breakPoint | minValue>breakPoint) ){
    	leds[i] -= CRGB( 0, 0, 20 );
    }
    else if( (i==2 | i==9 | i==16 | i==22 | i==27 | i==34 | i==47 | i==52 | i==57 | i==64) & (spectrumValue[2]<breakPoint | minValue>breakPoint) ){
    	leds[i] -= CRGB( 0, 0, 20 );
    }   
    else if( (i==3 | i==10 | i==17 | i==23 | i==28 | i==53 | i==58 | i==65) & (spectrumValue[3]<breakPoint | minValue>breakPoint) ){
    	leds[i] -= CRGB( 0, 0, 20 );
    }
    else if( (i==4 | i==11 | i==18 | i==24 | i==29 | i==36 | i==38 | i==42 | i==44 | i==54 | i==59 | i==66) & (spectrumValue[4]<breakPoint | minValue>breakPoint) ){
        leds[i] -= CRGB( 0, 0, 20 );
    }
    else if( (i==5 | i==12 | i==19 | i==30 | i==39 | i==48 | i==60 | i==67) & (spectrumValue[5]<breakPoint | minValue>breakPoint) ){
     	leds[i] -= CRGB( 0, 0, 20 );
    }
    else if( (i==6 | i==13 | i==31 | i==40 | i==49 | i==61 | i==68) & (spectrumValue[6]<breakPoint | minValue>breakPoint) ){
        leds[i] -= CRGB( 0, 0, 20 );
    }    
	  // else raise brightness if no thresholds met
	  else{
		  leds[i] += CRGB( 0, 0, 20 ); 
	  }
}

void msgeq7GetValues() {
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
       	}

       	// strobe pin high the low to go to next channel on msgeq7  
       	digitalWrite(strobePin, HIGH);

    }
     
    //for(int i=0;i<numberOfChannels;i++){Serial.print(spectrumValue[i]);Serial.print(" ");}
    //Serial.println();
    //Serial.println(breakPoint);
}
