int analogPin = 0; // MSGEQ7 OUT
int strobePin = 2; // MSGEQ7 STROBE
int resetPin = 4; // MSGEQ7 RESET

int spectrumValue[7];
int spectrumMax[7];
int spectrumMin[7];


#define RED 9
#define GREEN 10
#define BLUE 11

float maxBright = 100;
float brightness = 100;
float rgb[3];

void setup() {
  Serial.begin(57600);
  // put your setup code here, to run once:

  pinMode(analogPin, INPUT);
  // Write to MSGEQ7 STROBE and RESET
  pinMode(strobePin, OUTPUT);
  pinMode(resetPin, OUTPUT);
  
  analogReference(DEFAULT); // 5V
 
  // Set startup values for pins
  digitalWrite(resetPin, LOW);
  digitalWrite(strobePin, HIGH);
  
  // MSGEQ7 is noisy so a noise floor has to be established -- tailor these to your preference / amplification setup
  spectrumMin[0] = 150;
  spectrumMin[1] = 200;
  spectrumMin[2] = 120;
  spectrumMin[3] = 80;
  spectrumMin[4] = 120;
  spectrumMin[5] = 1023;//BAND 6 NEVER GIVES RELIABLE RESULTS DESPITE TRYING MUTLTIPLE CHIPS
  spectrumMin[6] = 80;
  
  
  // noise ceilings -- tailor these to your preference / amplification setup
  spectrumMax[0] = 500;
  spectrumMax[1] = 800;
  spectrumMax[2] = 500;
  spectrumMax[3] = 600;
  spectrumMax[4] = 800;
  spectrumMax[5] = 1023;//BAND 6 IS FUCKED FOR SOME REASON
  spectrumMax[6] = 300;
  
  
  
  
  //init pin modes
  for (int k=0; k<3; k++) {
    pinMode(RED + k, OUTPUT);
    rgb[k]=255;
    analogWrite(RED + k, rgb[k] * brightness/256);
  }
}

void loop() {
  // put your main code here, to run repeatedly: 
  digitalWrite(resetPin, HIGH);
  digitalWrite(resetPin, LOW);
  
  
  for (int i = 0; i < 7; i++) {
    if(i == 5) i ++;
    digitalWrite(strobePin, LOW);
    delayMicroseconds(30); // Allow output to settle
 
    spectrumValue[i] = analogRead(analogPin);
    
    Serial.print(spectrumValue[i]);
    if(spectrumValue[i] < 100) Serial.print("  ");
    else if(spectrumValue[i] < 1000) Serial.print(" ");
    
    Serial.print(" ");
 
    digitalWrite(strobePin, HIGH);
   }
   Serial.println();
   
   //brightness = map(spectrumValue[0],spectrumMin[0],spectrumMax[0],0,maxBright);
   //if(brightness < 0) brightness = 0;
   //Serial.println(brightness);
   
   rgb[0] = map(spectrumValue[0],spectrumMin[0],spectrumMax[0],0,maxBright);
   rgb[1] = map(spectrumValue[4],spectrumMin[4],spectrumMax[4],0,maxBright);
   rgb[2] = map(spectrumValue[2],spectrumMin[2],spectrumMax[2],0,maxBright);
   
   for (int k=0; k<3; k++) { // for all three colours
     if(rgb[k] < 0) rgb[k] = 0;
     analogWrite(RED + k, rgb[k]);
   }
}
