#include "Bus.h"
#include "Z80.h"
#include "TestMemory_64K.h"
#include "TestPeripheral.h"

Bus::Bus()
{
	cpu.connectBus(this);


	memDevices.push_back(new TestMemory_64K);
	perDevices.push_back(new TestPeripheral);

	for (auto& i : memDevices) i->connectBus(this);
	for (auto& i : perDevices) i->connectBus(this);
}

Bus::~Bus()
{
	for (auto& i : memDevices) delete i;
	for (auto& i : perDevices) delete i;
}

uint8_t Bus::readMemory(uint16_t addr)
{
	accepted = false;

	for (uint8_t i = 0; i < memDevices.size(); i++) {
		dataBuff = memDevices[i]->readMemory(accepted, addr);
		if (accepted)
			return dataBuff;
	}

	return 0xFF;
}

void Bus::writeMemory(uint16_t addr, uint8_t data)
{
	accepted = false;

	for (uint8_t i = 0; i < memDevices.size(); i++) {
		memDevices[i]->writeMemory(accepted, addr, data);
		if (accepted)
			break;
	}
}

uint8_t Bus::readPeripheral(uint16_t addr)
{
	accepted = false;

	for (uint8_t i = 0; i < perDevices.size(); i++) {
		dataBuff = perDevices[i]->readPeripheral(accepted, addr);
		if (accepted)
			return dataBuff;
	}

	return 0xFF;
}

void Bus::writePeripheral(uint16_t addr, uint8_t data)
{
	accepted = false;

	for (uint8_t i = 0; i < perDevices.size(); i++) {
		perDevices[i]->writePeripheral(accepted, addr, data);
		if (accepted)
			break;
	}
}
