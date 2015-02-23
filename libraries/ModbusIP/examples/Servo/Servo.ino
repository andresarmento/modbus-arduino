/*
  Modbus-Arduino Example - Servo (Modbus IP)
  Copyright by André Sarmento Barbosa
  http://github.com/andresarmento/modbus-arduino
*/
 
#include <SPI.h>
#include <Ethernet.h>
#include <Modbus.h>
#include <ModbusIP.h>
#include <Servo.h>

// Modbus Registers Offsets (0-9999)
const int SERVO_HREG = 100; 
// Used Pins
const int servoPin = 9;

// ModbusIP object
ModbusIP mb;
// Servo object
Servo servo; 

void setup() {
    // The media access control (ethernet hardware) address for the shield
    byte mac[] = { 0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED };  
    // The IP address for the shield
    byte ip[] = { 192, 168, 1, 120 };   
    // Config Modbus IP 
    mb.config(mac, ip);
    // Attaches the servo pin to the servo object
    servo.attach(servoPin); 
    // Add SERVO_HREG register - Use addHreg() for analog outpus or to store values in device 
    mb.addHreg(SERVO_HREG, 127);
}

void loop() {
   //Call once inside loop() - all magic here
   mb.task();
   
   //Attach switchPin to SWITCH_ISTS register     
   servo.write(mb.Hreg(SERVO_HREG));
   delay(15);
}
