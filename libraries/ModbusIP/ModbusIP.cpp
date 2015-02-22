/*
    ModbusIP.cpp - Source for Modbus IP Library
    Copyright (C) 2015 André Sarmento Barbosa
*/
#include "ModbusIP.h"

ModbusIP::ModbusIP() {
    uint16_t _port = 502;  //Default Modbus IP port
}

bool ModbusIP::config(uint8_t *mac, IPAddress ip, IPAddress dns, IPAddress gateway, IPAddress subnet) {
    Ethernet.begin(mac, ip, dns, gateway, subnet);
    server = new EthernetServer(_port);
    server->begin();

    return true;
}

void ModbusIP::setPort(uint16_t port) {
    this->_port = port;
}

void ModbusIP::proc() {
    EthernetClient client = server->available();
    if (client) {
        while (client.connected()) {
            if (client.available()) {
                char c = client.read();
                if (c == '\n') {
                    client.println("Beleza!");
                    break;
                }
            }
        }
        delay(1);
        client.stop();
    }
}
