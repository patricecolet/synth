#define DEBUG 0
#include <Ramp.h>                           // include library
rampInt myRamp;                                // new ramp object (ramp<unsigned char> by default)
int glide = 0;
#include "SPI.h"
/*
 * Control of a DAC MCP4822 over SPI
 *
 * MCP4822 Pin
 * 1 Power Supply 5V
 * 2 Chip Select - Arduino Pin 8 - teensy 2.0 Pin 0
 * 3 Clock - Arduino Pin 13 - teensy 2.0 Pin 1
 * 4 Data In - Arduino Pin 11 - teensy 2.0 Pin 2
 * 5 Latch - Arduino Pin 9 - ground
 * 6 VOut B
 * 7 Ground
 * 8 VOut A
 */
bool gate = 0;
int playing = -1;
int nextNote;
int nextVel;
int velCutOn;
int velCutOff;
byte LFOrate;
const int LFOratePin=PIN_C7;
const int gatePin=PIN_D1;
const int cvPin=0;
const int cutOffPin=1;
void setup() {
  Serial.begin(115200);
  usbMIDI.setHandleNoteOn(myNoteOn);
  usbMIDI.setHandleNoteOff(myNoteOff);
  usbMIDI.setHandleControlChange(myControlChange);
  
 SPI.begin();

 pinMode(cvPin, OUTPUT);
 pinMode(gatePin, OUTPUT);
 pinMode(cutOffPin, OUTPUT);
 pinMode(LFOratePin, OUTPUT);

}
void writeNote() {
 int val;
 if (glide > 0 ) {
  val = myRamp.update();
 } else val = nextNote; 
   writeSPI(cvPin,0,val);
   writeSPI(cutOffPin,0,nextVel);
}

void writeSPI(byte channel,byte cs,int v) {
 int gain = 2; //range = 2* VREF
 int shutdown = 1;
 byte lowByte = v & 0xff;
 byte highByte = ((v >> 8) & 0xff) | channel << 7 | gain << 5 | shutdown << 4;
 
  digitalWrite(cs, 0);

  SPI.transfer(highByte);
  SPI.transfer(lowByte);

  digitalWrite(cs, 1); 
}
void loop() {
  // The handler functions are called when usbMIDI reads data.  They
  // will not be called automatically.  You must call usbMIDI.read()
  // regularly from loop() for usbMIDI to actually read incoming
  // data and run the handler functions as messages arrive.
  usbMIDI.read();
  digitalWrite(gatePin,gate);
  writeNote();
  analogWrite(LFOratePin, LFOrate);
//  digitalWrite(LFOratePin, 0); 
}

void myNoteOn(byte channel, byte note, byte velocity) {
  if (note < 121) {
    int noteVoltage = 0;
    if (note > 0) noteVoltage = 4000 *  (float) note/120.0;
    nextNote = noteVoltage;
    myRamp.go (noteVoltage,glide,SINUSOIDAL_OUT);
    if (glide == 0) myRamp.go(noteVoltage);
    if (gate == 0) playing = note;
    gate = 1;
    if (velCutOn != 0) {
      float freqVoltage = 4000.0 *  ((float) velocity/127.0)  *  ((float) velCutOn/127.0);
    nextVel = freqVoltage;
    }  else nextVel = 0;
  }
}

void myNoteOff(byte channel, byte note, byte velocity) {
  float voltage ;
  if (note < 121) {  
  if (note != playing) {
    voltage = 4000 * (float) playing/120;
    myRamp.go (voltage,glide,SINUSOIDAL_OUT);
    nextNote = voltage;
  }
  else gate = 0;
  if (velCutOff > 0) {
    voltage = 4000 * (float) velocity/128 * (float) velCutOff/128;
  //} else voltage = nextVel
    nextVel = voltage;
    }
  }
}

void myControlChange(byte channel, byte control, byte value) {
//  Serial.print("Control Change, ch=");
//  Serial.print(channel, DEC);
//  Serial.print(", control=");
//  Serial.print(control, DEC);
//  Serial.print(", value=");
//  Serial.println(value, DEC);
  if (control == 100) {
    if (value < 1) gate = 0;
    else gate = 1;
  }
  else if (control == 5) {
    glide = value * 10;
  }
  else if (control == 14) {
    velCutOn = value;
  }
  else if (control == 15) {
    velCutOff = value;
  }
  else if (control == 20) {
    LFOrate = value;
  }
}
