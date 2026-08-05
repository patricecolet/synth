#include <MCP48xx.h>
MCP4822 dac0(4);
MCP4822 dac1(7);
MCP4822 dac2(8); 

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

bool gate;
const byte sawPin = 3;
const byte noisePin = 5;
const byte pulsewidthPin = 6;
const byte pulsePin = 9;
const byte gatePin = 2;

elapsedMillis lfoMod[] = {0, 0, 0, 0};
unsigned int lfoModTime[] = {300, 1000, 1500, 100};
byte lfoIndex[] = {0, 0, 0, 0};

byte com[3];

long tune = 0;
byte fine = 128;
byte note = 0;
void setup() {
//  set pwm frequency to 7812.5 Hz for D5 and D6
  TCCR0B = TCCR0B & B11111000 | B00000001;
//  set pwm frequency to 3921.16 Hz for D3 and D11
  TCCR2B = TCCR2B & B11111000 | B00000001;

  Serial.begin(115200); 
//  Serial.setTimeout(5000);
  Serial.println("PS101"); 
  
  pinMode(sawPin, OUTPUT);
  pinMode(pulsePin, OUTPUT);
  pinMode(noisePin, OUTPUT);
  pinMode(pulsewidthPin, OUTPUT);
  pinMode(gatePin, OUTPUT);
  analogWrite(sawPin,255);
  analogWrite(pulsePin,255);
  analogWrite(noisePin,255);
  analogWrite(pulsewidthPin,255);
  analogWrite(noisePin,255);
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
  }
  return pgm_read_byte(&waveTables[wav][lfoIndex[lfo]]);
}

void set_lfo(byte com[])
{
  for (byte i = 0; i < 3; ++i) {
    com[i] = Serial.parseInt();
    if (i < 3) { 
      int dot = Serial.read();  // throw away dot
      if (dot = 59) {
        if (com[0] == 0)  lfoModTime[com[1]] = com[2];
        //Serial.println(com[2]);
      }
    }
  }
}

void writeAnalog(byte p) {
  while (!Serial.available());
  byte v = (byte) Serial.parseInt();
  Serial.print("p");
  Serial.print(p);
  Serial.print(" ");
  Serial.println(v);
  analogWrite(p,255 - v);
}

void setCutoff() {  
  while (!Serial.available());
  long v = Serial.parseInt();
  Serial.print("c ");
  Serial.println(v);
  v = v * 4095 / 255;
  dac0.setVoltageB(v);
  dac0.updateDAC();
}

void setRes() {
  while (!Serial.available());
  long v = Serial.parseInt();
  Serial.print("r ");
  Serial.println(v);
  v = v * 4095 / 255;
  dac1.setVoltageA(v);
  dac1.updateDAC();
}

void setAmp() {
  while (!Serial.available());
  long v = Serial.parseInt();
  Serial.print("a ");
  Serial.println(v);
  v = v * 4095 / 255;
  dac2.setVoltageB(v);
  dac2.updateDAC();
}

void setTune(byte mode) {
  long v = Serial.parseInt();
  switch (mode) {
    case 0:
      Serial.print("t ");
      Serial.println(v);
      tune = v * 4095 / 255;
      break;
    case 1:
      Serial.print("t ");
      Serial.println(v);
      fine = v;
      break;
    case 2:
      Serial.print("t ");
      Serial.println(v);
      note = v * 4095 / 120;
    default:
      break;
  }
  dac0.setVoltageA(tune + fine + note);
  dac0.updateDAC();
}

void loop() {  
   if (Serial.available()) {
    byte command = Serial.read();
    switch (command) {
      case 116: // track "t"
        setTune(0);
        break;
      case 102: // track "f"
        setTune(1);
        break;
      case 78: // track "N"
        setTune(2);
        break;
      case 115: // track "s"
        writeAnalog(sawPin);
        break;
      case 112: // track "p"        
        writeAnalog(pulsePin);
        break;
      case 110: // track "n"
        writeAnalog(noisePin);
        break;
      case 119: // track "w"
        writeAnalog(pulsewidthPin);
        break;
      case 99: // track "c"
        setCutoff();
        break;
      case 114: // track "r"
        setRes();
        break;
      case 97: // track "a"
        setAmp();
        break;
      default:
        break;
    }
  }
}
