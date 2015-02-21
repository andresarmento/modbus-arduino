/*
    Modbus.h - Header for Modbus Base Library
    Copyright (C) 2014 André Sarmento Barbosa
*/
#include "Arduino.h"

#ifndef MODBUS_H
#define MODBUS_H

#define MAX_REGS     32
#define MAX_FRAME   128

typedef unsigned int u_int;

enum {
    FC_READ_COILS     = 0x01, //Read Coils (Output) Status 0xxxx
	FC_READ_INPUTS    = 0x02, //Discrete Input (Input Status) 1xxxx
    FC_READ_REGS      = 0x03, //Read Holding Registers 4xxxx
	FC_READ_INPUTREGS = 0x04, //Read Input Registers 3xxxx
	FC_WRITE_COIL     = 0x05, //Write Single Coil (Output) 0xxxx
    FC_WRITE_REG      = 0x06, //Preset Single Register 4xxxx
    FC_WRITE_COILS    = 0x0F, //Write Multiple Coils (Outputs) 0xxxx
    FC_WRITE_REGS     = 0x10, //Write block of contiguous registers 4xxxx
};

enum {
    REPLY_OFF    = 0x01,
    REPLY_ECHO   = 0x02,
    REPLY_NORMAL = 0x03,
};

typedef struct TRegister {
    word address;
    word value;
    struct TRegister* next;
} TRegister;

class Modbus {
    private:
        TRegister *_regs_head;
        TRegister *_regs_last;

        bool sReadCoils(word startreg, word numregs);
        bool sReadDiscreteInputs(word startreg, word numregs);
        bool sReadRegisters(word startreg, word numregs);
        bool sReadInputRegisters(word startreg, word numregs);
        bool sWriteSingleCoil(word reg, word status);
        bool sWriteSingleRegister(word reg, word value);
        bool sWriteCoils(word startreg, word numoutputs, byte bytecount);
        bool sWriteRegisters(word startreg, word numoutputs, byte bytecount);

        TRegister* searchRegister(word addr);

    protected:
        byte *_frame;
        byte  _len;
        byte  _reply;
        bool receivePDU(byte* frame);

    public:
        Modbus();

        void addReg(word address); //ok
        void addCoil(word offset); //ok
        void addDimp(word offset);
        void addIreg(word offset);
        void addHreg(word offset); //ok

        bool Reg(word address, word value); //ok
        bool Coil(word offset, bool value); //ok
        bool Dimp(word offset, bool value);
        bool Ireg(word offset, word value);
        bool Hreg(word offset, word value); //ok

        word Reg(word address); //ok
        bool Coil(word offset); //ok
        bool Dimp(word offset);
        word Ireg(word offset);
        word Hreg(word offset); //ok

};

#endif //MODBUS_H
