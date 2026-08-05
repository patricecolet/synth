/*
    This sketch demonstrates how we can output a value in both channels of MCP4822 or MCP4812 or MCP4802.
*/

#include <MCP48xx.h>

// Define the MCP4822 instance, giving it the SS (Slave Select) pin
// The constructor will also initialize the SPI library
// We can also define a MCP4812 or MCP4802
MCP4822 dac0(10);
MCP4822 dac1(9);
MCP4822 dac2(8);

// We define an int variable to store the voltage in mV so 100mV = 0.1V
int voltage = 0;

void setup() {
    // We call the init() method to initialize the instance

    dac0.init();
    dac1.init();
    dac2.init();

    // The channels are turned off at startup so we need to turn the channel we need on
    dac0.turnOnChannelA();
    dac0.turnOnChannelB();
    dac1.turnOnChannelA();
    dac1.turnOnChannelB();
    dac2.turnOnChannelA();
    dac2.turnOnChannelB();

    // We configure the channels in High gain
    // It is also the default value so it is not really needed
//    dac0.setGainA(MCP4822::High);
//    dac0.setGainB(MCP4822::High);
//    dac1.setGainA(MCP4822::High);
//    dac1.setGainB(MCP4822::High);
//    dac2.setGainA(MCP4822::High);
//    dac2.setGainB(MCP4822::High);
}

// We loop from 100mV to 2000mV for channel A and 4000mV for channel B
void loop() {
    // We set channel A to output 500mV
    dac0.setVoltageA(voltage);
    dac1.setVoltageA(voltage);
    dac2.setVoltageA(voltage);

    // We set channel B to output 1000mV
    dac0.setVoltageB(voltage * 2);
    dac1.setVoltageB(voltage * 2);
    dac2.setVoltageB(voltage * 2);

    // We send the command to the MCP4822
    // This is needed every time we make any change
    dac0.updateDAC();
    dac1.updateDAC();
    dac2.updateDAC();

    if (voltage * 2 > 5000) {
        voltage = 0;
    }

    voltage = voltage + 100;

    delay(1000);
}
