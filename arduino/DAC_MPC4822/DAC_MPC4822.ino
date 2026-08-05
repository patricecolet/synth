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

#include <MCP48xx.h>
MCP4822 dac0(4);
MCP4822 dac1(7);
MCP4822 dac2(8);


const int pwmPulsePin = 3;
const int noisePin = 5;
const int resPin = 6;

elapsedMillis dacMod;
unsigned int dacModTime = 500;
byte dacValue = 0;

elapsedMillis lfoMod[] = {0,0};
unsigned int lfoModTime[] = {300,1000};
byte lfoIndex[] = {0,0};

unsigned int triangle, sawtooth, pulsewidth, noise, pwm, cutoff, amp;

void setup() {
//  set pwm frequency to 7812.5 Hz for D5 and D6
//  TCCR0B = TCCR0B & B11111000 | B00000001;
//  set pwm frequency to 3921.16 Hz for D3 and D11
//  TCCR2B = TCCR2B & B11111000 | B00000010;

  Serial.begin(9600); 
    Serial.println("PS101"); 
  pinMode(pwmPulsePin, OUTPUT);
  pinMode(resPin, OUTPUT);
  pinMode(noisePin, OUTPUT);
  // put your setup code here, to run once:
    dac0.init();
    dac1.init();
    dac2.init();
    dac0.turnOnChannelA();
    dac0.turnOnChannelB();
    dac1.turnOnChannelA();
    dac1.turnOnChannelB();
    dac2.turnOnChannelA();
    dac2.turnOnChannelB();
}
byte lfo(byte lfo, byte wav, byte offset) {
  if (lfoMod[lfo] > lfoModTime[lfo]) {
    if (lfoIndex[lfo] > 255)  lfoIndex[lfo] = 0;
    lfoIndex[lfo]++;
    lfoMod[lfo] = 0;
  return pgm_read_byte(&waveTables[wav][lfoIndex[lfo]]);
}
}
void loop() {
  // put your main code here, to run repeatedly:

//   Write 0 1 2 3 4 5 volt on CV in DAC
  if (dacMod > dacModTime) {
    int dac = 1000 * dacValue++;
    dac0.setVoltageA(dac);
    dacMod = 0; 
    if (dacValue >= 5) dacValue = 0;
    dac0.updateDAC();
  }

    triangle =  lfo(0,0,lfo);
    sawtooth = lfo(0,6,lfo);
    pulsewidth = lfo(1,0,lfo);
    noise = lfo(1,6,lfo);;
    pwm = lfo(0,0,lfo);

    analogWrite(pwmPulsePin,127);
    analogWrite(noisePin,255);
    analogWrite(resPin,255);   
    dac0.setVoltageB(triangle*8);
    dac0.updateDAC();  
    dac1.setVoltageA(sawtooth*8);
    dac1.setVoltageB(pulsewidth*8);
    dac1.updateDAC(); 
    dac2.setVoltageA(noise*8);
    dac2.setVoltageB(pwm*8);
    dac2.updateDAC();
}
