/*
  Modbus-Arduino Example - Lamp (Modbus IP ESP8266 AT)
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
const int LAMP1_COIL = 100;
//Used Pins
const int ledPin = 13;

//ModbusIP object
ModbusIP mb;

void setup() {
    //Config Modbus IP
    mb.config(wifi, "your_ssid", "your_password");
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
