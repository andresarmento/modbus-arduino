/*
    ModbusIP_ESP8266.cpp - Source for Modbus IP ESP8266 AT Library
    Copyright (C) 2015 André Sarmento Barbosa
*/
#include "ModbusIP_ESP8266AT.h"


ModbusIP::ModbusIP() {

}

void ModbusIP::config(ESP8266 &wifi, String ssid, String password) {
    //Check errors
    //Enable serial log
    wifi.setOprToStationSoftAP();
    wifi.joinAP(ssid, password);
    wifi.enableMUX();
    wifi.startTCPServer(MODBUSIP_PORT);
    wifi.setTCPServerTimeout(MODBUSIP_TIMEOUT);
    _wifi = &wifi;
}

void ModbusIP::task() {
    uint8_t buffer[128] = {0};
    uint8_t mux_id;

	if (prev_conn) {
		_wifi->stopTCPServer();
		_wifi->enableMUX();
		_wifi->startTCPServer(MODBUSIP_PORT);
		_wifi->setTCPServerTimeout(10);
		prev_conn = false;
	}
    uint32_t raw_len = _wifi->recv(&mux_id, buffer, sizeof(buffer), MODBUSIP_TIMEOUT);

    if (raw_len > 7) {

        for (int i = 0; i < 7; i++)	_MBAP[i] = buffer[i]; //Get MBAP

        _len = _MBAP[4] << 8 | _MBAP[5];
        _len--; // Do not count with last byte from MBAP
        if (_MBAP[2] !=0 || _MBAP[3] !=0) return;  //Not a MODBUSIP packet
        if (_len > MODBUSIP_MAXFRAME) return;      //Length is over MODBUSIP_MAXFRAME

        _frame = (byte*) malloc(_len);

        for (int i=0; i < _len; i++) _frame[i] = buffer[7+i]; //Get Modbus PDU

        this->receivePDU(_frame);

        if (_reply != MB_REPLY_OFF) {
            //MBAP
            _MBAP[4] = (_len+1) >> 8;     //_len+1 for last byte from MBAP
            _MBAP[5] = (_len+1) & 0x00FF;

            buffer[4] = _MBAP[4];
            buffer[5] = _MBAP[5];

            for (int i=0; i<_len; i++)	buffer[i+7] = _frame[i];

            _wifi->send(mux_id, buffer, _len + 7);
        }

		prev_conn = true;

        free(_frame);
        _len = 0;
    }
}
