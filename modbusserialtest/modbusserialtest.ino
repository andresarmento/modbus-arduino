/*
  Modbus Lamp Test
 */
#include <Modbus.h>
#include <ModbusSerial.h>

ModbusSerial mb;

#define COIL1    10
#define COIL2    20
#define LAMP     30
#define TESTREG 100
#define DISCINP 200
#define INPREG  300

const int ledPin = 13;

long t1, t2;

void setup() {
    mb.config(&Serial, 19200, SERIAL_8N1);
    mb.setSlaveId(10);
    
    pinMode(ledPin, OUTPUT);
    
    mb.addCoil(COIL1);
    mb.addCoil(COIL2);
    mb.addCoil(LAMP);
    mb.addHreg(TESTREG);
    mb.addDimp(DISCINP);
    mb.addIreg(INPREG);
    
    mb.Hreg(TESTREG, 314);
    mb.Dimp(DISCINP, true);
    mb.Ireg(INPREG, 0);
    mb.Coil(COIL1, false);
    mb.Coil(COIL2, false);
    
    t1 = millis();
    t2 = millis();
}

void loop() {
   mb.proc();
   if (mb.Coil(COIL1) && mb.Coil(COIL2)) 
     mb.Coil(LAMP, true); 
   else 
     mb.Coil(LAMP, false); 
     
   digitalWrite(ledPin, mb.Coil(LAMP));
   
   if (millis() > t1 + 1000) {   //Increment each second
       t1 = millis();
       mb.Ireg(INPREG, mb.Ireg(INPREG)+1);
   }   
      
   if (millis() > t2 + 7000) {   //Change state each seven seconds
       t2 = millis();
       mb.Dimp(DISCINP, !mb.Dimp(DISCINP));
   }  
}
