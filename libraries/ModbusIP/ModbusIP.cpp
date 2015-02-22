/*
    ModbusIP.cpp - Source for Modbus IP Library
    Copyright (C) 2015 André Sarmento Barbosa
*/
#include "ModbusIP.h"

ModbusIP::ModbusIP():_server(MODBUSIP_PORT) {
}

bool ModbusIP::config(uint8_t *mac, IPAddress ip, IPAddress dns, IPAddress gateway, IPAddress subnet) {
    Ethernet.begin(mac, ip);
    _server.begin();
    return true;
}

bool ModbusIP::receive(byte* frame) {

}

void ModbusIP::proc() {
    EthernetClient client = _server.available();

    if (client) {
        if (client.connected()) {
            int i = 0;
            while (client.available()){
                MBAP[i] = client.read();
                i++;
                if (i==7) break;  //MBAP length has 7 bytes size
            }
			_len = MBAP[4] << 8 | MBAP[5];
			_len--;  // Do not count with last byte from MBAP

			if (_len > MODBUSIP_MAXFRAME) return;
			_frame = (byte*) malloc(_len);
			i = 0;
			while (client.available()){
			    _frame[i] = client.read();
                i++;
                if (i==_len) break;
			}

			if (this->receivePDU(_frame)) {
				for (i = 0 ; i < 7 ; i++) {
					client.write((byte)0);
				}
				for (i = 0 ; i < _len ; i++) {
					client.write(_frame[i]);
				}
			}

		    free(_frame);
	        _len = 0;
            client.stop();
        }
    }
}
