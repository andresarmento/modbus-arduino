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

class ModbusIP : public Modbus {
    private:
        uint16_t _port;
        EthernetServer* server;

    public:
        ModbusIP();
        bool config(uint8_t *mac, IPAddress ip, IPAddress dns, IPAddress gateway, IPAddress subnet);
        void setPort(uint16_t port);
        void proc();
};

#endif //MODBUSIP_H
