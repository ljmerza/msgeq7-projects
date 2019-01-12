/*------------------------------------------------------------
Created by: AdverseDeviant
Version 1.0
------------------------------------------------------------*/
 
/*------------------------------------------------------------
Variables
------------------------------------------------------------*/
int analogPin = 0; // pin for msgeq7 input
int strobePin = 13; // msgeq7 strobe pin for cycling through channels
int resetPin = 12; // reset pin of the msgeq7
int spectrumValue = 0; // current spectrum value
int highLowDelay = 0; // delay of resetting msgeq7
int strobeDelay_USec = 15; // delay to settle input recording in microseconds
int numberOfChannels = 7; // number of channels in
int divider = 100;
int multiplier = 1;

int multiBlack[4] = {2,3,4,5};
int multiRed[4] = {6,7,8,9};

int muxChannel[9][4]={  
    {0,0,0,0}, //channel 0
    {1,0,0,0}, //channel 1
    {0,1,0,0}, //channel 2
    {1,1,0,0}, //channel 3
    {0,0,1,0}, //channel 4
    {1,0,1,0}, //channel 5
    {0,1,1,0}, //channel 6
    {1,1,1,0}, //channel 7
    {0,0,0,1}, //channel 8
  };
 
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
 
 for(int i=0;i<8;i++) {
   pinMode(i+2,OUTPUT);
 }
 
 analogReference(DEFAULT);
 
 // reset msgeq7
 digitalWrite(resetPin, LOW);
 digitalWrite(strobePin, HIGH);
} // void setup()
 
 
/*------------------------------------------------------------
Loop method. Resets msgeq7 and captures value of the
7 channels on the msgeq7.
------------------------------------------------------------*/
void loop()
{
 digitalWrite(resetPin, HIGH);
 delay(highLowDelay);
 digitalWrite(resetPin, LOW);
 
 for (int i=0; i<numberOfChannels;i++) {
   // start reading channel by changing strobe to low
   digitalWrite(strobePin, LOW);
   // allows input to settle to get accurate reading
   delayMicroseconds(strobeDelay_USec);
   // read value of current pin from msgeq7
   spectrumValue = analogRead(analogPin);
   
   // print out value to serial monitor
   Serial.print(spectrumValue);
   Serial.print(" ");
   
   for(int j=0;j<4;j++) {
     digitalWrite(multiBlack[j], muxChannel[i][j]);
   } // for(int j=0;j<4;j++)
   
   for(int n=1+multiplier;n<10+multiplier;n++) {
     for(int k=0;k<n-multiplier;k++) {
       if(spectrumValue > divider*n) {
         for(int m=0;m<4;m++) {
           digitalWrite(multiRed[m], muxChannel[k][m]);
         } // for(int m=0;m<4;m++)u
       } // if(spectrumValue[i] > 30*n)
     } // for(int k=0;k<n;k++)
   } // for(int n=1;n<10;n++)
   
  // strobe pin high the low to go to next channel on msgeq7  
   digitalWrite(strobePin, HIGH);
  
 } // for (int i = 0; i < numberOfChannels; i++)
 Serial.println();
 
} // void loop()

