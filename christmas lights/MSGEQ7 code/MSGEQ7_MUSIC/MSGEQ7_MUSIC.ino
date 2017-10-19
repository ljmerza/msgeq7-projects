/*------------------------------------------------------------
Created by: Leonardo Merza
v2.21
------------------------------------------------------------*/

/*------------------------------------------------------------
Variables
------------------------------------------------------------*/
int   analogPin           = A5;   // pin for msgeq7 input
int   strobePin           = 13;   // msgeq7 strobe pin for cycling through channels 
int   resetPin            = 12;   // reset pin of the msgeq7

int   numberOfChannels    = 7;    // number of channels in msgeq7
int   spectrumValue[7]    = {};   // array to store the 7 values of the 7 channels
int   LEDpins[7]          = {2,3,4,5,6,7,8};
int   resetDelay_USec     = 80;   // delay of resetting MSGEQ7
int   strobeDelay_USec    = 15;   // delay to settle input recording in microseconds

int   counter             = 0;    // counter of averaging breakPoint
int   sumValues           = 0;    // current sum of values
int   breakPoint          = 0;  // lower cut off point of output number
float sumValueChange      = 1.2;  // multiply average value by this
int   minValue            = 120;  // minimum value that triggers lights

int delayWithoutSerial    = 25; // amount to delay without serial - serial slows down loop time
/*------------------------------------------------------------
Setup Method.  Initializes all pins 
------------------------------------------------------------*/
void setup() {
  // open usb serial port
  Serial.begin(9600);
  // turn on pins for msgeq7
  pinMode(analogPin,  INPUT);
  pinMode(strobePin,  OUTPUT);
  pinMode(resetPin,   OUTPUT);
 
  for (int i=0; i<numberOfChannels; i++) {
    pinMode(LEDpins[i], OUTPUT);
  } // for loop to initialize all leds

  // reset msgeq7
  digitalWrite(resetPin,  LOW);
  digitalWrite(strobePin, HIGH);
} // void setup()

/*------------------------------------------------------------
Loop method. Resets msgeq7 and captures value of the
7 channels on the msgeq7.
------------------------------------------------------------*/
void loop(){
  // reset msgeq7
  digitalWrite(resetPin, HIGH);
  delayMicroseconds(resetDelay_USec);
  digitalWrite(resetPin, LOW);

  // reset sumValues
  sumValues = 0;

  for (int i = 0; i < numberOfChannels; i++) {
    // start reading channel by changing strobe to low
    digitalWrite(strobePin, LOW);
    // allows input to settle to get accurate reading
    delayMicroseconds(strobeDelay_USec);
    // read value of current pin from msgeq7
    spectrumValue[i] = analogRead(analogPin);

    // print out value to serial monitor and sum value
    //Serial.print(spectrumValue[i]);
    //Serial.print(" ");

    // sum values of channels
    sumValues += spectrumValue[i];

    // strobe pin high the low to go to next channel on msgeq7  
    digitalWrite(strobePin, HIGH);
  }  // get value of each channel and add them together

  // get average of all values and add multiplier
  breakPoint = (sumValues / numberOfChannels) * sumValueChange;

  // delay code because it's too fast for mechanical relays @16Mhz
  delay(delayWithoutSerial);

  // display break point
  //Serial.print("Break Point: ");
  //Serial.println(breakPoint);

  for (int i = 0; i < numberOfChannels; i++) {   
    // if value is higher then break point the turn LED on else turn off
    if(spectrumValue[i] > breakPoint && minValue < breakPoint) {
      digitalWrite(LEDpins[i], HIGH);
    } // if readIn is higher than breakPoint and minimum value then turn on LED
    else {
      digitalWrite(LEDpins[i], LOW);
    } // else turn off LED
  } // for each input LED see if on or off
} // void loop()
