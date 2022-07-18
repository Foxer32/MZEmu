#include "TestBus.h"

TestBus::TestBus()
{
	cpu.connectBus(this);

	for (auto& i : mem) i = 0x00;
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
	uint8_t operation = cpu.C;

	if (operation == 2)
		std::cout << cpu.E;

	if (operation == 9)
	{
		addr = (cpu.D << 8) | cpu.E;

		do
		{
			std::cout << readMemory(addr++);
		} while (readMemory(addr) != '$');
	}


	return 0xFF;
}

void TestBus::writePeripheral(uint16_t addr, uint8_t data)
{
	testFinished = true;
}
