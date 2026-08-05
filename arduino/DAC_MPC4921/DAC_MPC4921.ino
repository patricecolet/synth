#include <elapsedMillis.h>

#include <MCP492X.h>


#define PIN_SPI_CHIP_SELECT 8 // Or any pin you'd like to use

MCP492X myDac(PIN_SPI_CHIP_SELECT);


const int pwmPulsePin = 9;
const int pwmLevelPin = 10;
const int sawtoothPin = 5;
const int trianglePin = 6;
const int noisePin = 3;
elapsedMillis pwmMod;
unsigned int pwmModTime = 1000;
byte pwmModValue = 0;
byte pwmPulseValue = 0;
byte pwmLevelValue = 0;
elapsedMillis dacMod;
unsigned int dacModTime = 100000;
byte dacValue = 0;
bool up= true;
byte pwmEnable;

void setup() {
//  set pwm frequency to 7812.5 Hz for D5 and D6
  TCCR0B = TCCR0B & B11111000 | B00000001;
//  set pwm frequency to 7812.5 Hz for D9 and D10
  TCCR1B = TCCR1B & B11111000 | B00000001;
//  set pwm frequency to 3921.16 Hz for D3 and D11
  TCCR2B = TCCR2B & B11111000 | B00000010;
  
  pinMode(pwmPulsePin, OUTPUT);
  pinMode(pwmLevelPin, OUTPUT);
  pinMode(sawtoothPin, OUTPUT);
  pinMode(trianglePin, OUTPUT);
  pinMode(noisePin, OUTPUT);
  // put your setup code here, to run once:
  myDac.begin();
}

void loop() {
  // put your main code here, to run repeatedly:

  // Write any value from 0-4095
  if (dacMod > dacModTime) {
    int dac = (4096/5) * dacValue++;
    if (dac>4095) dac = 4095;
    myDac.analogWrite(dac);
    dacMod = 0; 
    if (dacValue >= 6) dacValue = 0;
  }
  
  if (pwmMod > pwmModTime) {
    if (up) {
      if (pwmModValue < 256) {
        pwmModValue++;
        pwmPulseValue++;
        if (pwmModValue == 255) {
          up = false;
          pwmLevelValue = pwmLevelValue + 20;
        }
      }
    }
    else if (pwmModValue > 0){
      pwmModValue--;
        pwmPulseValue--;
      if (pwmModValue == 0) up = true;
    }
    byte sawtooth = constrain(pwmPulseValue, 0, 255); 
    byte triangle = constrain(255-pwmPulseValue, 0, 255); 
    analogWrite(trianglePin,pwmPulseValue); 
    analogWrite(sawtoothPin,pwmPulseValue);
    analogWrite(pwmLevelPin,pwmPulseValue); 
    analogWrite(pwmPulsePin,127);
    analogWrite(noisePin,255-pwmPulseValue);   
    if (pwmPulseValue>255) pwmPulseValue = 0; 
    analogWrite(pwmPulsePin,127);
    if (pwmLevelValue>255) pwmLevelValue = 0;
//    analogWrite(trianglePin,triangle); 
//    analogWrite(sawtoothPin,sawtooth);    
//    analogWrite(pwmLevelPin,255-pwmLevelValue);  
//    analogWrite(noisePin,pwmPulseValue);  
//    analogWrite(noisePin,up*255);  
    pwmMod = 0;  
  }
  
//    analogWrite(pwmLevelPin,127);
}
