/*
    ModbusIP_ENC28J60.cpp - Source for Modbus IP ENC28J60 Library
    Copyright (C) 2015 André Sarmento Barbosa
*/
#include "ModbusIP_ENC28J60.h"

byte Ethernet::buffer[MODBUSIP_MAXFRAME];

ModbusIP::ModbusIP() {
    ether.hisport = MODBUSIP_PORT;
}

void ModbusIP::config(uint8_t *mac) {
    ether.begin(sizeof Ethernet::buffer, mac, ENC28J60_CS);
    ether.dhcpSetup();
}

void ModbusIP::config(uint8_t *mac, uint8_t * ip) {
    ether.begin(sizeof Ethernet::buffer, mac, ENC28J60_CS);
    ether.staticSetup(ip);
}

void ModbusIP::config(uint8_t *mac, uint8_t * ip, uint8_t * dns) {
    ether.begin(sizeof Ethernet::buffer, mac, ENC28J60_CS);
    ether.staticSetup(ip, 0, dns);
}

void ModbusIP::config(uint8_t *mac, uint8_t * ip, uint8_t * dns, uint8_t * gateway) {
    ether.begin(sizeof Ethernet::buffer, mac, ENC28J60_CS);
    ether.staticSetup(ip, gateway, dns);
}

void ModbusIP::config(uint8_t *mac, uint8_t * ip, uint8_t * dns, uint8_t * gateway, uint8_t * subnet) {
    ether.begin(sizeof Ethernet::buffer, mac, ENC28J60_CS);
    ether.staticSetup(ip, gateway, dns, subnet);
}

void ModbusIP::task() {
    word len = ether.packetReceive();
    word pos = ether.packetLoop(len);

    if (pos) {
        int i = 0;
        while (i < 7) {
            _MBAP[i] = Ethernet::buffer[pos+i];
             i++;
        }

        _len = _MBAP[4] << 8 | _MBAP[5];
        _len--; // Do not count with last byte from MBAP
        if (_MBAP[2] !=0 | _MBAP[3] !=0) return;   //Not a MODBUSIP packet
        if (_len > MODBUSIP_MAXFRAME) return;      //Length is over MODBUSIP_MAXFRAME

        _frame = (byte*) malloc(_len);
        i = 0;
        while (i < _len){
            _frame[i] = Ethernet::buffer[pos+7+i];  //Forget MBAP and take just modbus pdu
            i++;
        }

        this->receivePDU(_frame);

        if (_reply != MB_REPLY_OFF) {
            //MBAP
            _MBAP[4] = (_len+1) >> 8;     //_len+1 for last byte from MBAP
            _MBAP[5] = (_len+1) & 0x00FF;

            BufferFiller bfill = ether.tcpOffset();
            bfill.emit_raw((const char *)_MBAP, 7);
            bfill.emit_raw((const char *)_frame, _len);
#ifdef TCP_KEEP_ALIVE
            ether.httpServerReplyAck ();
            ether.httpServerReply_with_flags(bfill.position(), TCP_FLAGS_ACK_V|TCP_FLAGS_PUSH_V); 
#else
            ether.httpServerReply(bfill.position());
#endif

        }

        free(_frame);
        _len = 0;
    }

}
