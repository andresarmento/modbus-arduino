/*
  Modbus-Arduino Example - Servo (Modbus IP ESP8266 AT)
  Copyright by André Sarmento Barbosa
  http://github.com/andresarmento/modbus-arduino
*/

#include <ESP8266.h>
#include <SoftwareSerial.h>
#include <Modbus.h>
#include <ModbusIP_ESP8266AT.h>
#include <Servo.h>

SoftwareSerial wifiSerial(2 , 3);
ESP8266 wifi(wifiSerial, 9600);

// Modbus Registers Offsets (0-9999)
const int SERVO_HREG = 100;
// Used Pins
const int servoPin = 9;

// ModbusIP object
ModbusIP mb;
// Servo object
Servo servo;

void setup() {
    //Config Modbus IP
    mb.config(wifi, "your_ssid", "your_password");
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
