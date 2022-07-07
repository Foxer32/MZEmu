#include "TestPeripheral.h"

TestPeripheral::TestPeripheral()
{
	for (auto& i : mem) i = 0x00;
}

uint8_t TestPeripheral::readPeripheral(bool& accepted, uint16_t addr)
{
	accepted = true;
	return mem[addr];
}

void TestPeripheral::writePeripheral(bool& accepted, uint16_t addr, uint8_t data)
{
	accepted = true;
	mem[addr] = data;
}
