#pragma once
#include "Device.h"

class IPeripheral : public Device
{
public:
	virtual uint8_t readPeripheral(bool& accepted, uint16_t addr) = 0;
	virtual void writePeripheral(bool& accepted, uint16_t addr, uint8_t data) = 0;
};
