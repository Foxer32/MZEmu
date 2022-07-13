#include "TestBus.h"

TestBus::TestBus()
{
	cpu.connectBus(this);

	for (auto& i : mem) i = 0x00;;
}

TestBus::~TestBus()
{

}

uint8_t TestBus::readMemory(uint16_t addr)
{
	return mem[addr];
}

void TestBus::writeMemory(uint16_t addr, uint8_t data)
{
	mem[addr] = data;
}

uint8_t TestBus::readPeripheral(uint16_t addr)
{
	return addr >> 8;
}

void TestBus::writePeripheral(uint16_t addr, uint8_t data)
{
	mem[addr] = data;
}
