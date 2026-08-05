

#include "Tlc5940.h"
const int tlc5940PowerPins[2] = {4,5};
//    -  Put the longer leg (anode) of the LEDs in the +5V and the shorter leg
//         (cathode) in OUT(0-15).
//    -  +5V from Arduino -> TLC pin 21 and 19     (VCC and DCPRG)
//    -  GND from Arduino -> TLC pin 22 and 27     (GND and VPRG)
//    -  digital 3        -> TLC pin 18            (GSCLK)
//    -  digital 9        -> TLC pin 24            (XLAT)
//    -  digital 10       -> TLC pin 23            (BLANK)
//    -  digital 11       -> TLC pin 26            (SIN)
//    -  digital 13       -> TLC pin 25            (SCLK)

#include <elapsedMillis.h>


// Waves
#include "wave/noise256.h"
#include "wave/ramp256.h"
#include "wave/saw256.h"
#include "wave/sine256.h"
#include "wave/tri256.h"
#include "wave/pulse8.h"
#include "wave/pulse16.h"
#include "wave/pulse64.h"
#include "wave/sq256.h"

// Wave table pointers
const byte *waveTables[] = {sine256, ramp256, saw256, tri256, pulse8, pulse16, pulse64, sq256, noise256};

#define NUM_WAVES (sizeof(waveTables) / sizeof(byte *))

#include <MCP48xx_soft.h>
MCP4822 dac0(8);
//MCP4822 dac1(7);


const int pwmPulsePin = 0;
const int noisePin = 1;
const int saw1Pin = 2;
const int tri1Pin = 3;
const int pwm1Pin = 4;
const int saw2Pin = 5;
const int tri2Pin = 6;
const int pwm2Pin = 7;
const int resPin  = 8;
const int vcaPin  = 9;

elapsedMillis dacMod;
unsigned int dacModTime = 1000;
byte dacValue = 0;

elapsedMillis lfoMod;
unsigned int lfoModTime = 300;
unsigned int lfoIndex = 0;

unsigned int triangle, sawtooth, pulsewidth, cutoff, amp;

void setup() {
 //turn TCL5940 power on
  Tlc.init();
//  for (int i=0; i<2; i++){     
//    pinMode(tlc5940PowerPins[i], OUTPUT);
//    digitalWrite(tlc5940PowerPins[i], i);
//  }

  Serial.begin(9600); 
    Serial.println("PS101"); 
  pinMode(pwmPulsePin, OUTPUT);
  pinMode(resPin, OUTPUT);
  pinMode(noisePin, OUTPUT);
  // put your setup code here, to run once:
    dac0.init();
//    dac1.init();
    dac0.turnOnChannelA();
    dac0.turnOnChannelB();
//    dac1.turnOnChannelA();
//    dac1.turnOnChannelB();
}

void loop() {
  // put your main code here, to run repeatedly:
    Tlc.clear();
//   Write 0 1 2 3 4 5 volt on CV in DAC
  if (dacMod > dacModTime) {
    int dac = 1000 * dacValue++;
    dac0.setVoltageA(dac);
    dacMod = 0; 
    if (dacValue >= 5) dacValue = 0;
    dac0.updateDAC();
  }
  if (lfoMod > lfoModTime) {
    if (lfoIndex > 255)  lfoIndex = 0;
    triangle = pgm_read_byte(&waveTables[3][lfoIndex]);
    sawtooth = waveTables[3][lfoIndex + 80];
    pulsewidth = waveTables[3][lfoIndex + 160]; 
    lfoIndex++;
    lfoMod = 0;
    }
    
    Tlc.set(pwmPulsePin, 2048);
    Tlc.set(noisePin, triangle*8);
    Tlc.set(saw1Pin, 1024);
    
    while (Tlc.update());
//    dac0.setVoltageA(1000);
//    dac0.updateDAC();
}
