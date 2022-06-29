#pragma once
#include <cstdint>
#include <vector>

//#include "Device.h"

#include "IMemory.h"
#include "IPeripheral.h"

#include "Z80.h"
#include "TestMemory_64K.h"
#include "IBus.h"

class Bus : public IBus
{
public:

	Bus();
	~Bus();

	//Z80 cpu;
	std::vector<IMemory*> memDevices;
	std::vector<IPeripheral*> perDevices;

	virtual uint8_t readMemory(uint16_t addr) override;
	virtual void writeMemory(uint16_t addr,uint8_t data) override;
	virtual uint8_t readPeripheral(uint16_t addr) override;
	virtual void writePeripheral(uint16_t addr, uint8_t data) override;
private:
	bool accepted = false;
	uint8_t dataBuff = 0x00;
};

