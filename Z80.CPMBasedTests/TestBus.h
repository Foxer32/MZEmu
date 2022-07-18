#pragma once
#include "IBus.h"
#include <iostream>

class TestBus : public IBus
{
public:

	TestBus();
	~TestBus();

	virtual uint8_t readMemory(uint16_t addr) override;
	virtual void writeMemory(uint16_t addr, uint8_t data) override;
	virtual uint8_t readPeripheral(uint16_t addr) override;
	virtual void writePeripheral(uint16_t addr, uint8_t data) override;

	uint8_t mem[64 * 1024];

	bool testFinished = false;
};
