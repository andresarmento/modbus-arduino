/*
  Modbus-Arduino Example - Servo (Modbus Serial)
  Copyright by André Sarmento Barbosa
  http://github.com/andresarmento/modbus-arduino
*/
 
#include <Modbus.h>
#include <ModbusSerial.h>
#include <Servo.h>

// Modbus Registers Offsets (0-9999)
const int SERVO_HREG = 100; 
// Used Pins
const int servoPin = 9;

// ModbusSerial object
ModbusSerial mb;
// Servo object
Servo servo; 

void setup() {
    // Config Modbus Serial (port, speed, byte format) 
    mb.config(&Serial, 38400, SERIAL_8N1);
    // Set the Slave ID (1-247)
    mb.setSlaveId(10); 
    
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
