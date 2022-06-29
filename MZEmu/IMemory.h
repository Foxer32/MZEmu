#pragma once
#include "Device.h"

class IMemory : public Device
{
public:
	virtual uint8_t readMemory(bool &accepted, uint16_t addr) =0;
	virtual void writeMemory(bool& accepted, uint16_t addr, uint8_t data) =0;
};
