#pragma once
#include "IPeripheral.h"

class TestPeripheral : public IPeripheral
{
public:
	TestPeripheral();
	uint8_t mem[64 * 1024];

	virtual uint8_t readPeripheral(bool& accepted, uint16_t addr) override;
	virtual void writePeripheral(bool& accepted, uint16_t addr, uint8_t data) override;
};