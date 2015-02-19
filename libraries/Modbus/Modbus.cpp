/*
    Modbus.cpp - Source for Modbus Base Library
    Copyright (C) 2014 André Sarmento Barbosa
*/
#include "Modbus.h"

Modbus::Modbus() {
    _regs_head = 0;
    _regs_last = 0;
}

TRegister* Modbus::search(word address) {
    TRegister *reg = _regs_head;
    //if there is no register configured, bail
    if(reg == 0) return(0);
    //scan through the linked list until the end of the list or the register is found.
    //return the pointer.
    do {
        if (reg->address == address) return(reg);
        reg = reg->next;
	} while(reg);
	return(0);
}

void Modbus::addReg(word address) {
    TRegister *newreg;

	newreg = (TRegister *) malloc(sizeof(TRegister));
	newreg->address = address;
	newreg->value		= 0;
	newreg->next		= 0;

	if(_regs_head == 0) {
        _regs_head = newreg;
        _regs_last = _regs_head;
    } else {
		//Assign the last register's next pointer to newreg.
	    _regs_last->next = newreg;
        //then make temp the last register in the list.
        _regs_last = newreg;
    }
}


void Modbus::addCoil(word offset) {
    this->addReg(offset + 1);
}

void Modbus::addDimp(word offset) {
    this->addReg(offset + 10001);
}

void Modbus::addIreg(word offset) {
    this->addReg(offset + 30001);
}

void Modbus::addHreg(word offset) {
    this->addReg(offset + 40001);
}

bool Modbus::Reg(word address, word value) {
    TRegister *reg;
    //search for the register address
    reg = this->search(address);
    //if found then assign the register value to the new value.
    if (reg) {
        reg->value = value;
        return true;
    } else
        return false;
}

bool Modbus::Coil(word offset, bool value) {
    return Reg(offset + 1, value?0xFF00:0x0000);
}

bool Modbus::Dimp(word offset, bool value) {
    return Reg(offset + 10001, value?0xFF00:0x0000);
}

bool Modbus::Ireg(word offset, word value) {
    return Reg(offset + 30001, value);
}

bool Modbus::Hreg(word offset, word value) {
    return Reg(offset + 40001, value);
}

word Modbus::Reg(word address) {
    TRegister *reg;
    reg = this->search(address);
    if(reg)
        return(reg->value);
    else
        return(NULL);
}

bool Modbus::Coil(word offset) {
    if (Reg(offset + 1) == 0xFF00) {
        return true;
    } else return false;
}

bool Modbus::Dimp(word offset) {
    if (Reg(offset + 10001) == 0xFF00) {
        return true;
    } else return false;
}

word Modbus::Ireg(word offset) {
    return Reg(offset + 30001);
}

word Modbus::Hreg(word offset) {
    return Reg(offset + 40001);
}


bool Modbus::setSlaveId(byte slaveId){
    _slaveId = slaveId;
    return true;
}

byte Modbus::getSlaveId() {
    return _slaveId;
}

bool Modbus::receivePDU(byte* frame) {
    byte fcode  = frame[0];
    word field1 = frame[1] << 8 | frame[2];
    word field2 = frame[3] << 8 | frame[4];

    switch (fcode) {
        case FC_WRITE_COIL:
            //field1 = reg, field2 = status
            this->sWriteSingleCoil(field1, field2);
        break;

        case FC_READ_COILS:
            //field1 = startreg, field2 = numregs
            this->sReadCoils(field1, field2);
        break;

        case FC_WRITE_REG:
            //field1 = reg, field2 = value
            this->sWriteSingleRegister(field1, field2);
        break;

        case FC_READ_REGS:
            //field1 = startreg, field2 = numregs
            this->sReadRegisters(field1, field2);
        break;

        case FC_READ_INPUTS:
            //field1 = startreg, field2 = numregs
            this->sReadDiscreteInputs(field1, field2);
        break;

        case FC_READ_INPUTREGS:
            //field1 = startreg, field2 = numregs
            this->sReadInputRegisters(field1, field2);
        break;
    }

    return true;
}

bool Modbus::sWriteSingleCoil(word reg, word status) {
    if (status == 0xFF00 || status == 0x0000)
        this->Coil(reg, status);
    _reply = REPLY_ECHO;
    return true;
}

bool Modbus::sWriteSingleRegister(word reg, word value) {
    if (value == 0xFF00 || value == 0x0000)
        this->Hreg(reg, value);
    _reply = REPLY_ECHO;
    return true;
}

bool Modbus::sReadCoils(word startreg, word numregs) {
    //Clean frame buffer
    free(_frame);
	_len = 0;

    //Determine the message length = function type, byte count and
	//for each group of 8 registers the message length increases by 1
	_len = 2 + numregs/8;
	if (numregs%8) _len++; //Add 1 to the message length for the partial byte.

    _frame = (byte *) malloc(_len);
    _frame[0] = FC_READ_COILS;
    _frame[1] = _len - 2;

    byte bitn = 0;
    word totregs = numregs;
    word i;
	while (numregs--) {
        i = (totregs - numregs) / 8;
		if (this->Coil(startreg))
			bitSet(_frame[2+i], bitn);
		else
			bitClear(_frame[2+i], bitn);
		//increment the bit index
		bitn++;
		if (bitn == 8) bitn = 0;
		//increment the register
		startreg++;
	}

    _reply = REPLY_NORMAL;
    return true;
}

bool Modbus::sReadDiscreteInputs(word startreg, word numregs) {
    //Clean frame buffer
    free(_frame);
	_len = 0;

    //Determine the message length = function type, byte count and
	//for each group of 8 registers the message length increases by 1
	_len = 2 + numregs/8;
	if (numregs%8) _len++; //Add 1 to the message length for the partial byte.

    _frame = (byte *) malloc(_len);
    _frame[0] = FC_READ_INPUTS;
    _frame[1] = _len - 2;

    byte bitn = 0;
    word totregs = numregs;
    word i;
	while (numregs--) {
        i = (totregs - numregs) / 8;
		if (this->Dimp(startreg))
			bitSet(_frame[2+i], bitn);
		else
			bitClear(_frame[2+i], bitn);
		//increment the bit index
		bitn++;
		if (bitn == 8) bitn = 0;
		//increment the register
		startreg++;
	}

    _reply = REPLY_NORMAL;
    return true;
}

bool Modbus::sReadRegisters(word startreg, word numregs) {
    //Clean frame buffer
    free(_frame);
	_len = 0;

	//calculate the query reply message length
	//for each register queried add 2 bytes
	_len = 2 + numregs * 2;

    _frame = (byte *) malloc(_len);
    _frame[0] = FC_READ_REGS;
    _frame[1] = _len - 2;

    word val;
    word i = 0;
	while(numregs--) {
		//retrieve the value from the register bank for the current register
		val = this->Hreg(startreg + i);
		//write the high byte of the register value
		_frame[2 + i * 2]  = val >> 8;
		//write the low byte of the register value
		_frame[3 + i * 2] = val & 0xFF;
		i++;
	}

    _reply = REPLY_NORMAL;
    return true;
}

bool Modbus::sReadInputRegisters(word startreg, word numregs) {
    //Clean frame buffer
    free(_frame);
	_len = 0;

	//calculate the query reply message length
	//for each register queried add 2 bytes
	_len = 2 + numregs * 2;

    _frame = (byte *) malloc(_len);
    _frame[0] = FC_READ_INPUTREGS;
    _frame[1] = _len - 2;

    word val;
    word i = 0;
	while(numregs--) {
		//retrieve the value from the register bank for the current register
		val = this->Ireg(startreg + i);
		//write the high byte of the register value
		_frame[2 + i * 2]  = val >> 8;
		//write the low byte of the register value
		_frame[3 + i * 2] = val & 0xFF;
		i++;
	}

    _reply = REPLY_NORMAL;
    return true;
}




