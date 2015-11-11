/*
    ModbusIP.h - Header for Modbus IP Library
    Copyright (C) 2015 André Sarmento Barbosa
*/
#include <Arduino.h>
#include <Modbus.h>
#include <SPI.h>
#include <Ethernet.h>

#ifndef MODBUSIP_H
#define MODBUSIP_H

#define MODBUSIP_PORT 	  502
#define MODBUSIP_MAXFRAME 200

//#define TCP_KEEP_ALIVE

class ModbusIP : public Modbus {
    private:
        EthernetServer _server;
        byte _MBAP[7];

    public:
        ModbusIP();
        void config(uint8_t *mac);
        void config(uint8_t *mac, IPAddress ip);
        void config(uint8_t *mac, IPAddress ip, IPAddress dns);
        void config(uint8_t *mac, IPAddress ip, IPAddress dns, IPAddress gateway);
        void config(uint8_t *mac, IPAddress ip, IPAddress dns, IPAddress gateway, IPAddress subnet);
        void task();
};

#endif //MODBUSIP_H

