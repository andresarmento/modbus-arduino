/*
  Modbus-Arduino Example - TempSensor (Modbus IP ENC28J60)
  Copyright by André Sarmento Barbosa
  http://github.com/andresarmento/modbus-arduino
*/

#include <EtherCard.h>
#include <Modbus.h>
#include <ModbusIP_ENC28J60.h>

//Modbus Registers Offsets (0-9999)
const int SENSOR_IREG = 100;
//Used Pins
const int sensorPin = A0;

//ModbusIP object
ModbusIP mb;

long ts;

void setup() {
    // The media access control (ethernet hardware) address for the shield
    byte mac[] = { 0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED };
    // The IP address for the shield
    byte ip[] = { 192, 168, 1, 120 };
    //Config Modbus IP
    mb.config(mac, ip);

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
