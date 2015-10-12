/*
  Modbus-Arduino Example - Switch (Modbus IP ESP8266 AT)
  Copyright by André Sarmento Barbosa
  http://github.com/andresarmento/modbus-arduino
*/

#include <ESP8266.h>
#include <SoftwareSerial.h>
#include <Modbus.h>
#include <ModbusIP_ESP8266AT.h>

SoftwareSerial wifiSerial(2 , 3);
ESP8266 wifi(wifiSerial, 9600);

//Modbus Registers Offsets (0-9999)
const int SWITCH_ISTS = 100;
//Used Pins
const int switchPin = 5;

//ModbusIP object
ModbusIP mb;

void setup() {
    //Config Modbus IP
    mb.config(wifi, "your_ssid", "your_password");
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
