/*
  Modbus-Arduino Example - TempSensor (Modbus IP ESP8266 AT)
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
const int SENSOR_IREG = 100;
//Used Pins
const int sensorPin = A0;

//ModbusIP object
ModbusIP mb;

long ts;

void setup() {
    //Config Modbus IP
    mb.config(wifi, "your_ssid", "your_password");

    // Add SENSOR_IREG register - Use addIreg() for analog Inputs
    mb.addIreg(SENSOR_IREG);

    ts = millis();
}

void loop() {
   //Call once inside loop() - all magic here
   mb.task();

   //Read each two seconds
   if (millis() > ts + 2000) {
       ts = millis();
       //Setting raw value (0-1024)
       mb.Ireg(SENSOR_IREG, analogRead(sensorPin));
   }
}
