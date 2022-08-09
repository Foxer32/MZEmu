#pragma once
#include "Z80.h"

class IBus
{
public:
	Z80 cpu;

	virtual uint8_t readMemory(uint16_t addr) = 0;
	virtual void writeMemory(uint16_t addr, uint8_t data) = 0;
	virtual uint8_t readPeripheral(uint16_t addr) = 0;
	virtual void writePeripheral(uint16_t addr, uint8_t data) = 0;
};
