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

// output parameters
// 0 = ctl
// 1 = lfo level
// 2 = lfo num
// 3 = lfo wave
// 4 = lfo offset
// 5 = lfo reset
// 6 = adsr lvl
// 7 = adsr num
// 8 = key lvl
;
byte pitch[] = {0, 128, 0, 0, 0, 0, 0, 0, 0};
byte trilvl[] = {0, 128, 0, 0, 0, 0, 0, 0, 0};
byte sawlvl[] = {0, 128, 0, 0, 0, 0, 0, 0, 0};
byte  pulselvl[] = {0, 128, 0, 0, 0, 0, 0, 0, 0};
byte noiselvl[] = {0, 128, 0, 0, 0, 0, 0, 0, 0};
byte pwm[] = {0, 128, 0, 0, 0, 0, 0, 0, 0};
byte cutoff[] = {0, 128, 0, 0, 0, 0, 0, 0, 0};
byte res[] = {0, 128, 0, 0, 0, 0, 0, 0, 0};
byte amp[] = {0, 128, 0, 0, 0, 0, 0, 0, 0};

long smooth[] = {0, 0, 0, 0, 0, 0, 0, 0, 0};
long finalValue[] = {0, 0, 0, 0, 0, 0, 0, 0, 0};

byte *param[] = {pitch, trilvl, sawlvl, pulselvl, noiselvl, pwm, cutoff, res, amp};
int cv = 60;
bool gate;
const byte pitchPin[] = {1, 0};
const byte triPin[] = {0, 10};
const byte sawPin[] = {0, 5};
const byte noisePin[] = {0, 6};
const byte pwmPulsePin[] = {0, 9};
const byte pulsePin[] = {0, 3};
const byte cutoffPin[] = {1, 1};
const byte resPin[] = {2, 0};
const byte ampPin[] = {2, 1};
const byte gatePin[] = {0, 2};
const byte *pinMap[] = {pitchPin, triPin, sawPin, pulsePin, noisePin, pwmPulsePin, cutoffPin, resPin, ampPin, gatePin}; 

elapsedMillis lfoTimer[] = {0, 0, 0, 0};
long lfoModTime[] = {300, 1000, 1500, 100};
byte lfoIndex[] = {0, 0, 0, 0};

byte com[3];

void setup() {
//  set pwm frequency to 7812.5 Hz for D5 and D6
  TCCR0B = TCCR0B & B11111000 | B00000001;
//  set pwm frequency to 3921.16 Hz for D3 and D11
  TCCR2B = TCCR2B & B11111000 | B00000001;

  Serial.begin(115200); 
//  Serial.setTimeout(5);
  Serial.println("PS101"); 
  for (int i = 0 ; i <= 8; i++) {
    if (pinMap[i][0] == 0) pinMode(i, OUTPUT);
  }
  dac0.init();
  dac1.init();
  dac2.init();
  dac0.turnOnChannelA();
  dac0.turnOnChannelB();
  dac1.turnOnChannelA();
  dac1.turnOnChannelB();
//  dac2.turnOnChannelA();
//  dac2.turnOnChannelB();
}

byte lfo(byte lfo, byte wav, byte offset) {
  int incr = 1;
  long thisTime = lfoModTime[lfo];
  if (lfoModTime[lfo] < 100) {
    incr = 2;
    thisTime *= 2;
  }
  if (lfoTimer[lfo] > thisTime) {
    if (lfoIndex[lfo] > 255)  lfoIndex[lfo] = 0;
    lfoIndex[lfo] += incr;
    lfoTimer[lfo] = 0;
  }
  return pgm_read_byte(&waveTables[wav][lfoIndex[lfo]]);
}
//
//int env(int adsr) {
//
//  return adsr;
//}
//
//void setDac(MCP4822 dac, bool ab, int res) {
//  if (ab) dac.setVoltageB(res);
//  else dac.setVoltageA(res);
//  dac.updateDAC();
//}

void smoothValue(byte i) {
//  int s = constrain((int) abs((finalValue[i] - smooth[i])/4),1,1024); 
  int s = constrain(abs((finalValue[i] - smooth[i])/16),1,1024); 
  if (finalValue[i] < smooth[i])  smooth[i]-= s;
  else if (finalValue[i] > smooth[i])  smooth[i]+= s;
//  else if (param[i][0] == smooth[i])  smooth[i] = finalValue[i];
}
void setOutputs() {
  long res;
  if (param[8][0] > 0) {
    for (int i = 0; i <= 8; i++) {
      res = param[i][0];
      byte lfoMod = param[i][1];
//apply modulators
      if (lfoMod != 128) {
        if (lfoMod > 128) { 
          lfoMod -= 128;
          lfoMod = lfo(param[i][2], param[i][3], param[i][4]) * lfoMod / 128;
          res = res + lfoMod;
        }
        else if (lfoMod < 128) {
          lfoMod = 128 - lfoMod;
          lfoMod = lfo(param[i][2], param[i][3], param[i][4]) * lfoMod / 128;
          res = res - lfoMod;
        } 
        res =  constrain(res, 0, 255); 
      } // else res *= 2;
//      if (param[i][6] != 128) {
//        res =  res + env(param[i][7]) * param[i][6] / 128; 
//      } else res *= 2;
// adjust CV
//      if ((i == 0) || (i == 6)) {
//        res =  (res + (cv - 60) * 4) / 8 ;
//      }
//          Serial.print("debug ");
//          Serial.print(i);
//          Serial.print(" ");
//          Serial.println(res);
//          delay(100);
      if (pinMap[i][0] == 0) {
        res = 255 - res;
        analogWrite(pinMap[i][1],res);
      }

      if (pinMap[i][0] > 0) {
        finalValue[i] = res * 4095 / 255;
        smoothValue(i);
        if (pinMap[i][0] == 1) {
          if (pinMap[i][1] == 1) {
            dac0.setVoltageB(finalValue[i]); //cutoff
            dac0.updateDAC();
          }
        }
        if (pinMap[i][0] == 2) {
          if (pinMap[i][1] == 1) dac1.setVoltageB(smooth[i]); // resonance
          else if (pinMap[i][1] == 0) dac1.setVoltageA(smooth[i]); // vca
          dac1.updateDAC();
        }
      }  
    }
  }
}


void set_param(byte com[])
{
  for (int i = 0; i < 3; ++i) {
    com[i] = (byte) Serial.parseInt();
    if (i < 3) { 
      byte dot = Serial.read();  // throw away dot
      if (dot = 47) {
        param[com[0]][com[1]] = com[2];
        //Serial.println(com[2]);
      }
    }
  }
}

void set_lfo(byte com[])
{
  for (byte i = 0; i < 3; ++i) {
    com[i] = Serial.parseInt();
//    if (i < 3) { 
      byte dot = Serial.read();  // throw away dot
      if (dot = 47) {
        if (com[0] == 0) {
          lfoModTime[com[1]] = -2000 * log10((float) (com[2] + 1) / 256);
        }
        Serial.print("lfo time:");
        Serial.println(lfoModTime[com[1]]);
      }
//    }
  }
}

void serialSend() {    
  for (int i = 0; i <= 8; i++) {
      Serial.print(i);
      Serial.print(" ");
      for (int j = 0; j < 7; j++) {
        Serial.print(param[i][j]);
        Serial.print(" ");       
      }  
      Serial.println(param[i][7]);
    }
}

void setPitch() {
  long pitch = Serial.parseInt();
  Serial.print("p ");
  Serial.println(pitch);
  pitch = pitch * 4095 / 120;
  dac0.setVoltageA(pitch);
  dac0.updateDAC();
}

void loop() {  
   if (Serial.available()) {
    byte command = Serial.read();
      switch (command) {
        case 58: // tracks ":" symbol
          set_param(com);
          break;
        case 108: // tracks "l" symbol
          set_lfo(com);
          break;
        case 61: // tracks "=" symbol
          serialSend();
          break;
        case 112: // tracks "p" symbol
          setPitch();
          break;
        default:
          break;
      }
    }
   setOutputs(); 
}
