/*
  Modbus-Arduino Example - Lamp (Modbus IP ENC28J60)
  Copyright by André Sarmento Barbosa
  http://github.com/andresarmento/modbus-arduino
*/

#include <EtherCard.h>
#include <Modbus.h>
#include <ModbusIP_ENC28J60.h>

//Modbus Registers Offsets (0-9999)
const int LAMP1_COIL = 100;
//Used Pins
const int ledPin = 9;

//ModbusIP object
ModbusIP mb;

void setup() {
    // The media access control (ethernet hardware) address for the shield
    byte mac[] = { 0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED };
    // The IP address for the shield
    byte ip[] = { 192, 168, 1, 120 };
    //Config Modbus IP
    mb.config(mac, ip);
    //Set ledPin mode
    pinMode(ledPin, OUTPUT);
    // Add LAMP1_COIL register - Use addCoil() for digital outputs
    mb.addCoil(LAMP1_COIL);
}

void loop() {
   //Call once inside loop() - all magic here
   mb.task();

   //Attach ledPin to LAMP1_COIL register
   digitalWrite(ledPin, mb.Coil(LAMP1_COIL));
}
