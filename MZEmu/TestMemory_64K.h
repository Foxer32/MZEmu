#pragma once
#include "IMemory.h"

class TestMemory_64K : public IMemory
{
public:
	TestMemory_64K();
	uint8_t mem[64 * 1024];

	virtual uint8_t readMemory(bool& accepted, uint16_t addr) override;
	virtual void writeMemory(bool& accepted, uint16_t addr, uint8_t data) override;
};
