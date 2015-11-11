/*
    ModbusIP_ENC28J60.h - Header for Modbus IP ENC28J60 Library
    Copyright (C) 2015 André Sarmento Barbosa
*/
#include <Arduino.h>
#include <Modbus.h>
#include <EtherCard.h>

#ifndef MODBUSIP_ENC28J60_H
#define MODBUSIP_ENC28J60_H

#define MODBUSIP_PORT 	  502
#define MODBUSIP_MAXFRAME 200

#define ENC28J60_CS 	10 //Default chip select pin
//#define TCP_KEEP_ALIVE

class ModbusIP : public Modbus {
    private:
        byte _MBAP[7];

    public:
        ModbusIP();
        void config(uint8_t *mac);
        void config(uint8_t *mac, uint8_t * ip);
        void config(uint8_t *mac, uint8_t * ip, uint8_t * dns);
        void config(uint8_t *mac, uint8_t * ip, uint8_t * dns, uint8_t * gateway);
        void config(uint8_t *mac, uint8_t * ip, uint8_t * dns, uint8_t * gateway, uint8_t * subnet);
        void task();
};

#endif //MODBUSIP_ENC28J60_H

