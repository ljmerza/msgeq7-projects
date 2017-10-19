/***********************************************************************************
Created by: Leonardo Merza
Version: 0.1
***********************************************************************************/

/***********************************************************************************
Imports
***********************************************************************************/
#include <LiquidCrystal.h>
#include <DHT.h>
#include <Adafruit_NeoPixel.h>
//#include "WS2812_Definitions.h" // added color names but probably wont need it if using HEX values

/***********************************************************************************
Variables
***********************************************************************************/
// RS, E, DB4, DB5, DB6, DB7 pins
int RSpin = 12;
int ePin = 11;
int db4Pin = 5;
int db5Pin = 4;
int db6Pin = 3;
int db7Pin = 2;

int lcdWidth = 16;
int lcdHeight = 2;

int batteryADC = A0; // ADC for battery check
int batteryValue = 0; // Value from ADC for battery

int dhtPin = 6;
int fanRelayPin = 7;
int ledPin = 8; // need to string all LEDs together

int micPin = A2;
int msgeq7AnalogPin = A1; // pin for msgeq7 input
int strobePin = 9; // msgeq7 strobe pin for cycling through channels
int resetPin = 10; // reset pin of the msgeq7

int spectrumValue = 0; // current spectrum value
int highLowDelay = 0; // delay of resetting msgeq7
int strobeDelay_USec = 15; // delay to settle input recording in microseconds
int numberOfChannels = 7; // number of channels in
int divider = 100;
int multiplier = 1;

LiquidCrystal lcd(RSpin, ePin, db4Pin, db5Pin, db6Pin, db7Pin);
DHT dht;


/*********************************************************************************/
void setup(){
  Serial.begin(9600);
  
  dht.setup(dhtPin);
  lcd.begin(lcdWidth, lcdHeight);
  
  pinMode(batteryADC, INPUT);
  pinMode(fanRelayPin, OUTPUT);
  
  pinMode(micPin, INPUT);

  // turn on pins for msgeq7
  pinMode(msgeq7AnalogPin, INPUT);
  pinMode(strobePin, OUTPUT);
  pinMode(resetPin, OUTPUT);
 
  for(int i=0;i<8;i++) {
    pinMode(i+2,OUTPUT);
  } // for(int i=0;i<8;i++)
  
  // reset msgeq7
  digitalWrite(resetPin, LOW);
  digitalWrite(strobePin, HIGH);
} // void setup(){

/***********************************************************************************/
void loop(){ 
  // get DHT value
  int temperature = dht.toFahrenheit(dht.getTemperature());
  
  // set lcd and write temp values
  lcd.setCursor(0, 0);
  lcd.print("Tmp:");
  lcd.setCursor(4, 0);
  lcd.print(temperature); 
  if(temperature > 100){
    lcd.setCursor(7, 0);
  }
  else{
    lcd.setCursor(6, 0);
  }
  lcd.print("F");
  
  // set batt value
  lcd.setCursor(8, 0);
  lcd.print("Batt:");
  lcd.setCursor(13, 0);
  lcd.print(batteryValue);
  
  // set volume value
  int micVol = analogRead(micPin);
  lcd.setCursor(0, 1);
  lcd.print("Vol:");
  lcd.setCursor(4, 1);
  lcd.print(micVol);

  if(micVol < 1000){
  	lcd.setCursor(7, 1);
  	lcd.print(" ");
  } // if(micVol < 1000)

  // set time?
  lcd.setCursor(8, 1);
  lcd.print("Tme:");
  lcd.setCursor(12, 1);
  lcd.print("2154");
 // msgeq7

}


void msgeq7(){
 digitalWrite(resetPin, HIGH);
 delay(highLowDelay);
 digitalWrite(resetPin, LOW);
 
 for (int i=0; i<numberOfChannels;i++) {
   // start reading channel by changing strobe to low
   digitalWrite(strobePin, LOW);
   // allows input to settle to get accurate reading
   delayMicroseconds(strobeDelay_USec);
   // read value of current pin from msgeq7
   spectrumValue = analogRead(msgeq7AnalogPin);
   // print out value to serial monitor
   Serial.print(spectrumValue);
   Serial.print(" ");
  // strobe pin high the low to go to next channel on msgeq7  
   digitalWrite(strobePin, HIGH);
 } // for (int i = 0; i < numberOfChannels; i++)
 
 Serial.println();
}
  
