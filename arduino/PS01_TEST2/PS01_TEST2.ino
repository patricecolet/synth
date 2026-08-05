#define DEBUG 0
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
// 4 = lfo delay
// 5 = lfo reset
// 6 = adsr lvl
// 7 = adsr num
// 8 = key lvl
// 9 = vel lvl
;
byte pitch[] = {128, 128, 0, 0, 0, 0, 0, 2, 255, 0};
byte trilvl[] = {128, 128, 0, 0, 0, 0, 0, 0, 0, 0};
byte sawlvl[] = {128, 128, 0, 0, 0, 0, 0, 0, 0, 0};
byte pulselvl[] = {128, 128, 0, 0, 0, 0, 0, 3, 0, 0};
byte noiselvl[] = {128, 128, 0, 0, 0, 0, 0, 0, 0, 0};
byte pwm[] = {128, 128, 0, 0, 0, 0, 0, 3, 0, 0};
byte cutoff[] = {255, 128, 0, 0, 0, 0, 0, 1, 255, 255};
byte res[] = {0, 128, 0, 0, 0, 0, 0, 0, 0, 0};
byte amp[] = {128, 128, 0, 0, 0, 0, 255, 0, 0, 0};
byte lfo1[] = {0, 128, 0, 0, 0, 0, 255, 0, 0, 0};
byte lfo2[] = {0, 128, 0, 0, 0, 0, 255, 0, 0, 0};
byte lfo3[] = {0, 128, 0, 0, 0, 0, 255, 0, 0, 0};
byte lfo4[] = {0, 128, 0, 0, 0, 0, 255, 0, 0, 0};
byte env1[] = {0, 128, 0, 0, 0, 0, 255, 0, 0, 0};
byte env2[] = {0, 128, 0, 0, 0, 0, 255, 0, 0, 0};
byte env3[] = {0, 128, 0, 0, 0, 0, 255, 0, 0, 0};
byte env4[] = {0, 128, 0, 0, 0, 0, 255, 0, 0, 0};

long smooth[] = {0, 0, 0, 0, 0, 0, 0, 0, 0};
long finalValue[] = {0, 0, 0, 0, 0, 0, 0, 0, 0};

byte *param[] = {pitch, trilvl, sawlvl, pulselvl, noiselvl, pwm, cutoff, res, amp, lfo1, lfo2, lfo3, lfo4, env1, env2, env3, env4};
int cv = 60;
byte vel;
bool gate,rel;
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

elapsedMillis debugTimer = 0;
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
  long timer =  lfoTimer[lfo] ; 
  long thisTime = lfoModTime[lfo] ;
  // hack to compensate slow loop
//  if (lfoModTime[lfo] < 100) {
//    incr = 200/lfoModTime[lfo];
//    thisTime *= 2;
//    if (lfoModTime[lfo] < 24) incr = 4;
//  }
  if (timer >= thisTime || timer < 1) {
    if (lfoIndex[lfo] >= 255)  lfoIndex[lfo] = 0;
    lfoIndex[lfo] += incr;
    lfoTimer[lfo] = 0;
  }
  return pgm_read_byte(&waveTables[wav][lfoIndex[lfo]]);
}
//
byte env(byte i) {
 
    long timer = envTimer[i]/64; 
  if (gate == 1) {
    if (rel == 0) {
    rel = 1;
    envTimer[i] = 0;
    timer = 0;
    }
    if (timer <= EGs[i][0]) {
      envState[i] = timer * 255 / EGs[i][0];
    }
    else if (timer > EGs[i][0] && timer <=  (EGs[i][1] +  EGs[i][0])) {
      envState[i] = 255 - (timer -  EGs[i][0]) *  (255 - EGs[i][2]) / EGs[i][1];
    }
    else if (timer > EGs[i][0] +  EGs[i][1]) envState[i] = EGs[i][2] ;
  }
   else if (gate == 0) {   
    if (timer > EGs[i][3] && rel == 1) {
      envTimer[i] = 0;
      rel = 0;
    }
    if (timer <= EGs[i][3] && rel == 0) envState[i] =  EGs[i][2] * (EGs[i][3] - timer) / EGs[i][3];
    if (timer > EGs[i][3] && rel == 0)  envState[i] = 0;
    }
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
float makeMod (byte lvl,float sig,byte mod, bool offset) {
  float res;
   if (offset == 1) {
  if (lvl > 128) {
    lvl = lvl - 128; 
    res = sig  * lvl / 127;
    sig -= res;
    sig +=  res * mod / 255;
  }
  else if (lvl < 128) {
   lvl = 128 - lvl;
   res = sig  * lvl / 127;
   sig += res;
   sig -= res * mod / 255;
   }
  }
  else if (offset == 0) {
   res = sig * lvl / 255;
   sig = res * mod / 255;
  }
  return sig;
}

void setOutputs() {
  float sig = param[id][0];
//apply modulators
  if (param[8][0] > 0) {  
   byte envMod = param[id][6];   
   if (envMod != 0 &&( id == 8 || id == 6)) {
    sig = makeMod(envMod, sig, env(param[id][7]),0); 
    byte lfoMod = param[id][1];
    byte lfoSig = lfo(param[id][2], param[id][3], param[id][4]);
    if (lfoMod != 128) {
      sig = makeMod(lfoMod,sig,lfoSig,1);  
    }      
   }
   //pitch settings
   byte keyMod = param[id][8];
//   if (id == 0) sig = cv*2;
//   } else if (keyMod != 128) {
      sig = makeMod(keyMod,sig,cv*2,0);  
//   } 
//   // velocity 
   byte velMod = param[id][9];
//   if (id == 8) sig = sig * vel / 127;
//   else sig = makeMod(velMod,sig,vel*2,0); 
      
   if (DEBUG == 1 && sig > 0 ) {
      if (debugTimer >= 6400 ) {
      Serial.print("DEBUG ");
      Serial.print("sig");
      Serial.print(id);
      Serial.print(" : ");
      Serial.println(sig); 
      debugTimer = 0; 
      }
    }
   if (pinMap[id][0] == 0) {
    
     sig = 255 - sig;
     if (sig < 255) analogWrite(pinMap[id][1],sig);
   }
   if (pinMap[id][0] > 0) {
     finalValue[id] = sig * 16;
     smoothValue(id);
     if (pinMap[id][0] == 1) {
       if (pinMap[id][1] == 1) dac0.setVoltageB(4095 - smooth[id]); //cutoff
       if (pinMap[id][1] == 0) dac0.setVoltageA((float)finalValue[id]); // pitch
       dac0.updateDAC();
     }
     if (pinMap[id][0] == 2) {
       if (pinMap[id][1] == 1) dac1.setVoltageB(smooth[id]); // resonance
       if (pinMap[id][1] == 0) dac1.setVoltageA(smooth[id]); // vca
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

void setNote(byte com[]) {
  for (byte i = 0; i < 3; i++) {
    com[i] = Serial.parseInt();
    byte dot = Serial.read();  // throw away dot
    if (dot == 47) {
      gate = com[0];
      cv = com[1];
      vel = com[2];
    }
  }
//  if(DEBUG == 1) {
//    Serial.print("DEBUG ");
//    Serial.print("gate: ");
//    Serial.println(gate);
//    Serial.print(" pitch: ");
//    Serial.println(cv);
//    Serial.print(" vel: ");
//    Serial.println(vel);   
//  }
  float pitch = cv * 32;
  dac0.setVoltageA(pitch);
  dac0.updateDAC();
  if (gate == 1) {
  for (int i= 0; i< 4; i++) {
    envTimer[i] = 0;
  }
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
      lfoModTime[com[1]] = -10000 * log10((float) (com[2] + 1) / 256);
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
        case 110: // tracks "n" symbol
          setNote(com);
          break;
        case 101: // tracks "e" symbol
          setEGs(com);
          break;
        default:
          break;
      }
    }
   setOutputs();
}
