#include "TestMemory_64K.h"

TestMemory_64K::TestMemory_64K()
{
	for (auto& i : mem) i = 0x00;
}

uint8_t TestMemory_64K::readMemory(bool& accepted, uint16_t addr)
{
	accepted = true;
	return mem[addr];
}

void TestMemory_64K::writeMemory(bool& accepted, uint16_t addr, uint8_t data)
{
	accepted = true;
	mem[addr] = data;
}
