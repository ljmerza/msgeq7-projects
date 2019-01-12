/*------------------------------------------------------------
Created by: AdverseDeviant
Version 1.0
------------------------------------------------------------*/

/*------------------------------------------------------------
Variables
------------------------------------------------------------*/
int LEDpins[7] = {2,3,4,5,6,7,8};

/*------------------------------------------------------------
Setup Method.  Initializes all pins 
------------------------------------------------------------*/
void setup() {
 
 // turn on each LED pin on arduino
 for (int i=0; i<7; i++) {
   pinMode(LEDpins[i], OUTPUT);
 } // for loop to initialize all leds
 
} // void setup()

void loop()
{
    // turn on each LED pin on arduino
   for (int i=0; i<7; i++) {
   digitalWrite(LEDpins[i], HIGH);
   delay(100);
   digitalWrite(LEDpins[i], LOW);
   delay(100);
   } // for loop to initialize all leds
} // void loop()
