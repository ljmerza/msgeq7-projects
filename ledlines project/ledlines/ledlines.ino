/*
 * Leonardo Merza
 * https://lmerza.com
 */

#include <FastLED.h>


/*
 * Variables
 */
#define NUM_LEDS 90
#define DATA_PIN 5 // data pin to LED strip
#define LINELENGTH 4 // length of each LED line
#define NUMBEROFLINES 14 // number of LED lines on strip at one time
#define EIGHTBIT 255
#define HUESPEED 5 // how fast the hue increments for each LED movement
#define LEDSPEED 0 // how fast the LED lines move in ms - affects MSGEQ7 reads

int hues[NUMBEROFLINES] = {0,18,36,54,72,91,109,127,145,163,182,200,281,236};
int positions[NUMBEROFLINES] = {0,7,14,21,28,35,42,49,56,63,70,77,84,90};
int values[NUMBEROFLINES];

int analogPin = 0; // pin for msgeq7 input
int strobePin = 13; // msgeq7 strobe pin for cycling through channels
int resetPin = 3; // reset pin of the msgeq7
int spectrumValue[7]; // array to store the 7 values of the 7 channels
int resetDelay_USec = 100; // delay of resetting MSGEQ7 in microseconds
int strobeDelay_USec = 40; // delay to settle input recording in microseconds
int numberOfChannels = 7; // number of channels in MSGEQ7

CRGB leds[NUM_LEDS]; // CRGB color object for fastLED library




/*
 * Setup LED strip and audio pins
 */
void setup () { 
	FastLED.addLeds<NEOPIXEL, DATA_PIN>(leds, NUM_LEDS);

	// turn on pins for msgeq7
	pinMode(analogPin, INPUT);
	pinMode(strobePin, OUTPUT);
	pinMode(resetPin, OUTPUT);

	// reset MSGEQ7 to ensure clean data
	digitalWrite(resetPin, LOW);
	digitalWrite(strobePin, HIGH);
}




/*
 * Main loop
 */
void loop () {

	// for each channel, read input and display value
	for(int i=0; i<numberOfChannels; i++){

		// reset to start over
		digitalWrite(resetPin, HIGH);
		delayMicroseconds(resetDelay_USec);
		digitalWrite(resetPin, LOW);

		digitalWrite(strobePin, LOW); // start reading channel by changing strobe to low
		delayMicroseconds(strobeDelay_USec); // allows input to settle to get accurate reading

		spectrumValue[i] = analogRead(analogPin); // read value of current pin from msgeq7
		values[i] = map(spectrumValue[1], 0, 1024, 0, EIGHTBIT); // map analog input to HSV value
		values[i+numberOfChannels] = map(spectrumValue[1], 0, 1024, 0, EIGHTBIT); // map analog input to HSV value

		digitalWrite(strobePin, HIGH); // strobe pin high then low to go to next channel on msgeq7  
	}




    // for each led line
    for ( unsigned int k = 0; k < NUMBEROFLINES; k++ ) {

	    // for each led in the led line add color
	    for( int i = 0; i < LINELENGTH; i++ ){
	        if ( positions[k] + i <  NUM_LEDS) {
	        	leds[positions[k] + i] = CHSV(hues[k], 255, values[k]);
	        }
	    }

	    // once all colors of that line are added show line
	    FastLED.show();

	    // turn off last LED, increment position
	    leds[positions[k]] = CRGB::Black;
	    positions[k]++;


	    // increment hue and check of hue max
	    hues[k] += HUESPEED;
	    if ( hues[k] > EIGHTBIT ) {
	    	hues[k] = 0;
	    }

	    // check for line position at end of strip
	    if ( positions[k] > NUM_LEDS + LINELENGTH - 2 ) {
	        // if at end of strip reset position
	        positions[k] = 0;
	    }

	}
	// delay until next led change
	delay(LEDSPEED);
}
