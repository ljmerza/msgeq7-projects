#include <FastLED.h>
#include <Adafruit_NeoPixel.h>
/*------------------------------------------------------------*/
#define LED_PIN     5
#define NUM_LEDS    70
#define BRIGHTNESS  128
#define LED_TYPE    WS2811
/*------------------------------------------------------------*/
CRGB leds[NUM_LEDS];
Adafruit_NeoPixel strip = Adafruit_NeoPixel(NUM_LEDS, LED_PIN, NEO_GRB + NEO_KHZ800);
/*------------------------------------------------------------*/
CRGBPalette16 currentPalette;
TBlendType    currentBlending;
/*------------------------------------------------------------*/
double first = 0;
double second = 0;
double initalValue = 50;
double tempValue = 1;
double value = initalValue;
double valueIncre = -1;
boolean isBlink = false;
/*------------------------------------------------------------*/
int analogPin = A5; // pin for msgeq7 input
int strobePin = 3; // msgeq7 strobe pin for cycling through channels 
int resetPin = 4; // reset pin of the msgeq7
int numberOfChannels = 7; // number of channels in msgeq7
int spectrumValue[7]; // array to store the 7 values of the 7 channels

int resetDelay = 80; // delay of resetting MSGEQ7 in microseconds
int strobeDelay_USec = 15; // delay to settle input recording in microseconds
/*------------------------------------------------------------*/
int breakPoint = 500; // lower cut off point of output number
int counter = 0; // counter of averaging breakPoint
int sumValues = 0; // current sum of values
int minValue =  130;
/*------------------------------------------------------------*/
float sumValueChange = 1.1; // multiply average value by this
/*------------------------------------------------------------*/
int switcherPin = 13;
int updatesPerSec = 0;
/*------------------------------------------------------------*/
void setup() {
	delay(3000); // safe bootup
	Serial.begin(9600);

	pinMode(switcherPin, INPUT);
	    
	//strip.begin();
	//strip.show(); // Initialize all pixels to 'off'

	FastLED.addLeds<LED_TYPE, LED_PIN>(leds, NUM_LEDS).setCorrection( TypicalLEDStrip );
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
/*------------------------------------------------------------*/
void loop() {

	if(true){
    	msgeq7Cycle();
  	} else {
    	static uint8_t startIndex = 0;
    	startIndex = startIndex + 1; /* motion speed */
    	fastLedCycle(startIndex);
    	FastLED.show();
    	//FastLED.delay(updatesPerSec);
  	}
}                               
 
/*------------------------------------------------------------*/
void msgeq7Cycle() {
   
  	for(int j=0;j<256*5;j++){ // 5 cycles of all colors on wheel
	    msgeq7();
	    
	    for(int i=0;i<strip.numPixels();i++) {
	        // first LEDs
	        if(i==0 | i==7 | i==14  | i==20 | i==25 | i==32 | i==35 | i==37 | i==41 | i==43 | i==45 | i==50 | i==55 | i==62){
		        if(spectrumValue[0]<breakPoint | minValue>breakPoint){
		           tempValue = value;
		           value = 1;
		           isBlink = true;
		        }
		    }
		    // second LEDs
		    else if(i==1 | i==8 | i==15 | i==21 | i==26 | i==33 | i==46 | i==51 | i==56 | i==63){
		        if(spectrumValue[1]<breakPoint | minValue>breakPoint){
		            tempValue = value;
		            value = 1;
		            isBlink = true;
		         }
		     }
		    // third LEDs
		    else if(i==2 | i==9 | i==16 | i==22 | i==27 | i==34 | i==47 | i==52 | i==57 | i==64){
		       if(spectrumValue[2]<breakPoint | minValue>breakPoint){
		           tempValue = value;
		            value = 1;
		            isBlink = true;
		       }
		    }   
		    // fourth LEDs
		    else if(i==3 | i==10 | i==17 | i==23 | i==28 | i==53 | i==58 | i==65){
		       if(spectrumValue[3]<breakPoint | minValue>breakPoint){
		        tempValue = value;
		         value = 1;
		         isBlink = true;
		       }
		    }
		    // fifth LEDs
		    else if(i==4 | i==11 | i==18 | i==24 | i==29 | i==36 | i==38 | i==42 | i==44 | i==54 | i==59 | i==66){
		        if(spectrumValue[4]<breakPoint | minValue>breakPoint){
		           tempValue = value;
		           value = 1;
		           isBlink = true;
		        }
		    }
	        //sixth LEDs
	        else if(i==5 | i==12 | i==19 | i==30 | i==39 | i==48 | i==60 | i==67){
	          	if(spectrumValue[5]<breakPoint | minValue>breakPoint){
	            	tempValue = value;
	             	value = 1;
	             	isBlink = true;
	        	}
	        }
	       // seventh LEDs
	       else if(i==6 | i==13 | i==31 | i==40 | i==49 | i==61 | i==68){
	         	if(spectrumValue[6]<breakPoint | minValue>breakPoint){
	            	tempValue = value;
	            	value = 1;
	           		isBlink = true;
	         	}
	       	}  
			strip.setPixelColor(i, Wheel(((i * 256 / strip.numPixels()) + j) & 255));
        
        	if(isBlink){
          		value = tempValue;
          		isBlink = false;
        	}
      	} // for strip
      	strip.show();
      
      	value=value+valueIncre;
      
      	if(value<1){
        	value=initalValue;
      	}
    } // for 256 * 5
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
      
       	sumValues += spectrumValue[i];
         
       	if(i>numberOfChannels-2){
        	// take average of values and reset counter/sum values
        	breakPoint = (sumValues/numberOfChannels)*sumValueChange;
        	sumValues = 0;
       	}

       // strobe pin high the low to go to next channel on msgeq7  
       digitalWrite(strobePin, HIGH);
    }
     
     for(int i=0;i<numberOfChannels;i++){Serial.print(spectrumValue[i]);Serial.print(" ");}
     Serial.println(breakPoint);
}

/*------------------------------------------------------------*/
void fastLedCycle(uint8_t colorIndex) {
	uint8_t brightness = 255;

  	for( int i = 0; i < NUM_LEDS; i++) {
    	leds[i] = ColorFromPalette( currentPalette, colorIndex, brightness, currentBlending);
    	colorIndex += 3;
	}
}