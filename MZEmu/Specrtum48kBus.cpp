#include "Specrtum48kBus.h"
#include <fstream>

Specrtum48kBus::Specrtum48kBus()
{
	for (auto& i : mem) i = 0x00;

	std::ifstream ifs;
	ifs.open("roms/48.rom", std::ifstream::binary);
	if (ifs.is_open())
	{
		ifs.read((char*)mem, 16 * 1024);
		ifs.close();
	}
}

Specrtum48kBus::~Specrtum48kBus()
{

}

uint8_t Specrtum48kBus::readMemory(uint16_t addr)
{
	return mem[addr];
}

void Specrtum48kBus::writeMemory(uint16_t addr, uint8_t data)
{
	if (addr > 0x3FFF)
		mem[addr] = data;
}

uint8_t Specrtum48kBus::readPeripheral(uint16_t addr)
{
	uint8_t result = video.attributePort;

	if ((addr & 0xFF) == 0xFE)
	{
		result = keyboard.getKey(addr >> 8);
		result = (audioIn > 0.3f) ? result | 0x40 : result & ~(0x40);
	}

	return result;
}

void Specrtum48kBus::writePeripheral(uint16_t addr, uint8_t data)
{
	if ((addr & 0xFF) == 0xFE)
	{
		speakerOut = (((data & 0x18) >> 3) * 0.25f) - 1;
		video.borderColor = data & 0x07;
	}
}

void Specrtum48kBus::reset(bool hardReset)
{
	cpu.reset(hardReset);
}

void Specrtum48kBus::clock()
{
	cpu.update();
	video.update();
	mixAudioInputs();
}

void Specrtum48kBus::mixAudioInputs()
{
	audioOut[0] = audioOut[1] = (speakerOut + audioIn) / 2;
}
