#include <FastLED.h>

#define NUM_LEDS 90
#define DATA_PIN 5

#define LINELENGTH 5
#define EIGHTBIT 255
#define HUESPEED 2

#define STRIPNUMB 7

unsigned int hues[STRIPNUMB] = {0,36,72,108,144,180,216};
unsigned int delays[STRIPNUMB];
unsigned int delayCounters[STRIPNUMB];
unsigned int positions[STRIPNUMB] = {0,13,26,39,52,65,78};
unsigned int actives[STRIPNUMB] = {1,1,1,1,1,1,1};
unsigned int values[STRIPNUMB] = {100,255,100,255,100,255,100};
unsigned int numbOfActives = 0;

boolean ledChange = false; // has any led color changed?

CRGB leds[NUM_LEDS];

void setup () { 
	FastLED.addLeds<NEOPIXEL, DATA_PIN>(leds, NUM_LEDS);
	Serial.begin(9600);
	for ( int i = 0; i < STRIPNUMB; i++ ) {
		delayCounters[i] = 0;
		delays[i] = 50;

	}

}


void loop () {
    // for each led line
    for ( unsigned int k = 0; k < STRIPNUMB; k++ ) {

		// if the led line is active then use it
		if ( actives[k] == 1) {
	        // if the led line delay counter is higher than the line's delay then activate led line
	        if ( delayCounters[k] > delays[k] ) {
	        	ledChange = true;
	        	// for each led in the led line add color
	        	for( int i = 0; i < LINELENGTH; i++ ){
	        		if ( positions[k] + i <  NUM_LEDS) {
	        			leds[positions[k] + i] = CHSV(hues[k], 255, values[k]);
	        		}
	        	}

	        	// turn off last LED, increment position, reset delay counter
	        	leds[positions[k]] = CRGB::Black;
	        	positions[k]++;
	        	delayCounters[k] = 0;

	        	// increment hue and check of hue max
	        	hues[k] += HUESPEED;
	        	if( hues[k] > EIGHTBIT ){
	        		hues[k] = 0;
	        	}

	        	// check for line position at end of strip
	        	if ( positions[k] > NUM_LEDS + LINELENGTH-1) {
	        		// if at end, reset position, active state to false, decrement number of actives
	        		positions[k] = 0;
	        		//actives[k] = 0;
	        		if ( numbOfActives > 0 ) {
	        			numbOfActives--;
	        		}
	        	}

	        	// if any led colors have changed then update led strip
	        	if ( ledChange ){
	        		FastLED.show();
	        		ledChange = false;
	        	}
	        }

	        // increment delay counter if hasn't met delay threshold
	        delayCounters[k]++;
	    } 
	}

	delay(1);

}