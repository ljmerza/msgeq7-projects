/*------------------------------------------------------------
Created by: Leonardo Merza
v1.6
------------------------------------------------------------*/

/*------------------------------------------------------------
Variables
------------------------------------------------------------*/
int analogPin = A5; // pin for msgeq7 input
int strobePin = 13; // msgeq7 strobe pin for cycling through channels 
int resetPin = 12; // reset pin of the msgeq7
int spectrumValue[7]; // array to store the 7 values of the 7 channels
int LEDpins[7] = {2,3,4,5,6,7,8};
int resetDelay = 1; // delay of resetting MSGEQ7
int breakPoint = 500; // lower cut off point of output number
int strobeDelay_USec = 15; // delay to settle input recording in microseconds
int counter = 0; // counter of averaging breakPoint
int sumValues = 0; // current sum of values
int numberOfChannels = 7; // number of channels in msgeq7
float sumValueChange = 1.5; // multiply average value by this
int minValue = 80; // minimum value that triggers lights

/*------------------------------------------------------------
Setup Method.  Initializes all pins 
------------------------------------------------------------*/
void setup() {
 // open usb serial port
 Serial.begin(9600);
 // turn on pins for msgeq7
 pinMode(analogPin, INPUT);
 pinMode(strobePin, OUTPUT);
 pinMode(resetPin, OUTPUT);
 
 // turn on each LED pin on arduino
 for (int i=0; i<numberOfChannels; i++) {
   pinMode(LEDpins[i], OUTPUT);
 } // for loop to initialize all leds

 // reset msgeq7
 digitalWrite(resetPin, LOW);
 digitalWrite(strobePin, HIGH);
} // void setup()

/*------------------------------------------------------------
Loop method. Resets msgeq7 and captures value of the
7 channels on the msgeq7.
------------------------------------------------------------*/
void loop(){
 digitalWrite(resetPin, HIGH);
 delay(resetDelay);
 digitalWrite(resetPin, LOW);

 for (int i = 0; i < numberOfChannels; i++) {
   // start reading channel by changing strobe to low
   digitalWrite(strobePin, LOW);
   // allows input to settle to get accurate reading
   delayMicroseconds(strobeDelay_USec);
   // read value of current pin from msgeq7
   spectrumValue[i] = analogRead(analogPin);
   // print out value to serial monitor
   Serial.print(spectrumValue[i]);
   Serial.print(" ");
   
   if(i < (numberOfChannels-1)) {
     // add  to average them
     sumValues += spectrumValue[i];
   } // if counter less than one less of  the number of channels
   else {
     // take average of values and reset counter/sum values
     breakPoint = (sumValues / numberOfChannels) * sumValueChange;
     Serial.print("Break Point: ");
     Serial.println(breakPoint);
     sumValues = 0;
   } // else set average of break point and reset value
   
   // if value is higher then break point the turn LED on else turn off
   if(spectrumValue[i] > breakPoint && minValue < breakPoint) {
     digitalWrite(LEDpins[i], HIGH);
   } // if readIn is higher than breakPoint then turn on LED
   else {
     digitalWrite(LEDpins[i], LOW);
   } // else turn off LED
   
   // strobe pin high the low to go to next channel on msgeq7  
   digitalWrite(strobePin, HIGH);
 } // for each input LED do this
 
} // void loop()
