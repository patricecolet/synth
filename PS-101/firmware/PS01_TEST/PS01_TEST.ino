#define DEBUG 1
#include <MCP48xx.h>
MCP4822 dac0(4);
MCP4822 dac1(7);
//MCP4822 dac2(8); 

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
const byte *waveTables[] = {sine256, ramp256, saw256, tri256, sq256};
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
// 9 = vel lvl
;
byte pitch[] = {0, 128, 0, 0, 0, 0, 0, 2, 0, 0};
byte trilvl[] = {0, 128, 0, 0, 0, 0, 0, 0, 0, 0};
byte sawlvl[] = {0, 128, 0, 0, 0, 0, 0, 0, 0, 0};
byte pulselvl[] = {0, 128, 0, 0, 0, 0, 0, 3, 0, 0};
byte noiselvl[] = {0, 128, 0, 0, 0, 0, 0, 0, 0, 0};
byte pwm[] = {0, 128, 0, 0, 0, 0, 0, 3, 0, 0};
byte cutoff[] = {0, 128, 0, 0, 0, 0, 0, 1, 255, 255};
byte res[] = {0, 128, 0, 0, 0, 0, 0, 0, 0, 0};
byte amp[] = {0, 128, 0, 0, 0, 0, 255, 0, 0, 0};

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


elapsedMillis envTimer[] = {0, 0, 0, 0};
long envModValue[] = {0, 0, 0, 0};
int adsr[] = {50, 300, 128, 50};
int *EGs[] = {adsr, adsr, adsr, adsr};
int envState[] = {0, 0, 0, 0};
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
  if (DEBUG == 1) {   
  Serial.println("DEBUG mode"); 
  }
  dac0.init();
  dac1.init();
//  dac2.init();
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
  // hack to compensate slow loop
  if (lfoModTime[lfo] < 100) {
    incr = 200/lfoModTime[lfo];
    thisTime *= 2;
    if (lfoModTime[lfo] < 24) incr = 4;
  }
  if (lfoTimer[lfo] >= thisTime || lfoTimer[lfo] < 30) {
    if (lfoIndex[lfo] > 255)  lfoIndex[lfo] = 0;
    lfoIndex[lfo] += incr;
    lfoTimer[lfo] = 0;
  }
  return pgm_read_byte(&waveTables[wav][lfoIndex[lfo]]);
}
//
byte env(byte i) {
  
  if (gate == 1) {
    long timer = envTimer[i]/64;
    if (timer <= EGs[i][0]) {
      envState[i] = timer * 255 / EGs[i][0];
    }
    else if (timer > EGs[i][0] && timer <=  (EGs[i][1] +  EGs[i][0])) {
      envState[i] = 255 - (timer -  EGs[i][0]) *  (255 - EGs[i][2]) / EGs[i][1];
    }
    else if ((timer > (EGs[i][1] +  EGs[i][0])) && timer <  (EGs[i][1] +  EGs[i][0] +  EGs[i][3])) {
      envState[i] = EGs[i][2] - (timer -  EGs[i][0] -  EGs[i][1]) *  EGs[i][2] / EGs[i][3];
    }
    else if (timer > (EGs[i][1] +  EGs[i][0] +  EGs[i][3])) {
      envState[i] = 0;
      gate = 0;
      envTimer[i]=0;
    }
          
//    if (DEBUG == 1) {
//      Serial.print("DEBUG ");
//      Serial.print("env: ");
//      Serial.println(envState[i]); 
//      delay(10); 
//    }
  } else envTimer[i] = 0;
  return envState[i];
}
//
//void setDac(MCP4822 dac, bool ab, int res) {
//  if (ab) dac.setVoltageB(res);
//  else dac.setVoltageA(res);
//  dac.updateDAC();
//}

void smoothValue(byte i) {
//  apply signal smoothing; 
  int s = constrain(abs((finalValue[i] - smooth[i])/16),1,1024); 
  if (finalValue[i] < smooth[i])  smooth[i]-= s;
  else if (finalValue[i] > smooth[i])  smooth[i]+= s;
}

int id = 0;
void setOutputs() {
  float res = 0;
//apply modulators
  if (param[8][0] > 0) {
   byte envMod = param[id][6];   
   if (envMod != 0) {
        res = env(param[id][7]) * (envMod / 127);
   byte lfoMod = param[id][1];
   if (lfoMod != 128) {
     if (lfoMod > 128) {
      lfoMod = lfoMod - 128; 
      float state = res  * lfoMod / 127;
        
       res -= state;
       res +=  state * lfo(param[id][2], param[id][3], param[id][4]) / 255;  
     }
     else if (lfoMod < 128) {
       lfoMod = 128 - lfoMod;
       res -= (lfo(param[id][2], param[id][3], param[id][4]) * lfoMod * res / 127) / 255;
     }
   }
 //     res =  constrain(res, 0, 255);
      
      res = res * param[id][0] / 511;
      
    if (DEBUG == 1 && res != 0) {
      Serial.print("DEBUG ");
      Serial.print("res: ");
      Serial.println(res); 
      delay(10); 
    }      
   }
   
   if (pinMap[id][0] == 0) {
     res = 255 - res;
     analogWrite(pinMap[id][1],res);
   }
   if (pinMap[id][0] > 0) {
     finalValue[id] = res * 16;
     smoothValue(id);
     if (pinMap[id][0] == 1) {
       if (pinMap[id][1] == 1) {
         dac0.setVoltageB(finalValue[id]); //cutoff
         dac0.updateDAC();
       }
     }
     if (pinMap[id][0] == 2) {
       if (pinMap[id][1] == 1) dac1.setVoltageB(smooth[id]); // resonance
       else if (pinMap[id][1] == 0) dac1.setVoltageA(smooth[id]); // vca
       dac1.updateDAC();
      }  
    }
    id++;
    if (id >= 9) id = 0;
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
  if(DEBUG == 1) {
    Serial.print("DEBUG ");
    Serial.print("pitch: ");
    Serial.println(pitch);   
  }
  pitch = pitch * 4095 / 120;
  dac0.setVoltageA(pitch);
  dac0.updateDAC();
  for (int i= 0; i< 4; i++) {
    envTimer[i] = 0;
  }
}

void setEGs(byte com[]) {
  for (byte i = 0; i < 3; i++) {
    com[i] = Serial.parseInt();
    byte dot = Serial.read();  // throw away dot
    if (dot == 47) {
      float res = ((exp(((float) com[2] + 1) / 256) - 1) * 3000) - 11.74;
      if (com[1] == 2) res = com[2];
      EGs[com[0]][com[1]] =  (int) res; 
      if (DEBUG == 1) {
        Serial.print("DEBUG ");
        Serial.print("env");
        Serial.print(com[0]);
        Serial.print(" a:");
        Serial.print(EGs[com[0]][0]);
        Serial.print(" d:");
        Serial.print(EGs[com[0]][1]);
        Serial.print(" s:");
        Serial.print(EGs[com[0]][2]);
        Serial.print(" r:");
        Serial.println(EGs[com[0]][3]);
        delay(10);
      }
    }
  }
}


void set_lfo(byte com[])
{
  for (byte i = 0; i < 3; ++i) {
    com[i] = Serial.parseInt();
    byte dot = Serial.read();  // throw away dot
    if (dot = 47) {
      lfoModTime[com[1]] = -2000 * log10((float) (com[2] + 1) / 256);
    }
    if (DEBUG == 1) {
      Serial.print("lfo time:");
      Serial.println(lfoModTime[com[1]]);
      delay(10);
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

void loop() {  
  bool trig = 0;
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
        case 101: // tracks "e" symbol
          setEGs(com);
          break;
        case 103: // tracks "g" symbol
          trig = 1;
          break;
        default:
          break;
      }
    }
   if (trig == 1) gate = 1;
   setOutputs();
   trig = 0; 
}
