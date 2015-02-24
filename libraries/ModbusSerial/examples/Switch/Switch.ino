/*
  Modbus-Arduino Example - Switch (Modbus Serial)
  Copyright by André Sarmento Barbosa
  http://github.com/andresarmento/modbus-arduino
*/
 
#include <Modbus.h>
#include <ModbusSerial.h>

//Modbus Registers Offsets (0-9999)
const int SWITCH_ISTS = 100; 
//Used Pins
const int switchPin = 3;

//ModbusSerial object
ModbusSerial mb;

void setup() {
    // Config Modbus Serial (port, speed, byte format) 
    mb.config(&Serial, 38400, SERIAL_8N1);
    // Set the Slave ID (1-247)
    mb.setSlaveId(10);  

    //Set ledPin mode
    pinMode(switchPin, INPUT);
    // Add SWITCH_ISTS register - Use addIsts() for digital inputs 
    mb.addIsts(SWITCH_ISTS);
}

void loop() {
   //Call once inside loop() - all magic here
   mb.task();
   
   //Attach switchPin to SWITCH_ISTS register     
   mb.Ists(SWITCH_ISTS, digitalRead(switchPin));
}
