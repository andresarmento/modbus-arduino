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

void ModbusIP::proc() {
    EthernetClient client = _server.available();

    if (client) {
        if (client.connected()) {
            int i = 0;
            while (client.available()){
                _MBAP[i] = client.read();
                i++;
                if (i==7) break;  //MBAP length has 7 bytes size
            }
			_len = _MBAP[4] << 8 | _MBAP[5];
			_len--;  // Do not count with last byte from MBAP

			if (_MBAP[2] !=0 | _MBAP[3] !=0) return;   //Not a MODBUSIP packet
			if (_len > MODBUSIP_MAXFRAME) return;      //Length is over MODBUSIP_MAXFRAME

			_frame = (byte*) malloc(_len);
			i = 0;
			while (client.available()){
			    _frame[i] = client.read();
                i++;
                if (i==_len) break;
			}

			if (this->receivePDU(_frame) && _reply != REPLY_OFF)  {
                //MBAP
                _MBAP[4] = _len >> 8;
                _MBAP[5] = _len | 0x00FF;
				for (i = 0 ; i < 7 ; i++) {
					client.write(_MBAP[i]);
				}
                //PDU Frame
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
