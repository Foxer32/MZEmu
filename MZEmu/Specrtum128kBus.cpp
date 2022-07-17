#include "Specrtum128kBus.h"

Specrtum128kBus::Specrtum128kBus()
{
	cpu.connectBus(this);
	video.connectBus(this);

	for (auto& i : rom) i = 0x00;
	for (auto& i : ram) i = 0x00;

	std::ifstream ifs;
	ifs.open("roms/128.rom", std::ifstream::binary);
	if (ifs.is_open())
	{
		ifs.read((char*)rom, 32 * 1024);
		ifs.close();
	}

	/*std::ifstream ifs2;
	ifs2.open("roms/DiagROMv56.rom", std::ifstream::binary);
	if (ifs2.is_open())
	{
		ifs2.read((char*)rom, 16 * 1024);
		ifs2.close();
	}*/
}

Specrtum128kBus::~Specrtum128kBus()
{

}

uint8_t Specrtum128kBus::readMemory(uint16_t addr)
{
	switch (addr >> 14)
	{
	case 0: // 0000 - 3FFF
		return rom[addr + ((port7FFD & 0x10) ? 0x4000 : 0)];
	case 1: // 4000 - 7FFF
		return ram[addr + ((port7FFD & 0x08) ? 0x18000 : 0x10000)];
	case 2: // 8000 - BFFF
		return ram[addr];
	case 3: // C000 - FFFF
		return ram[(addr - 0xC000) + (0x4000 * (port7FFD & 7))];
	}
}

void Specrtum128kBus::writeMemory(uint16_t addr, uint8_t data)
{
	switch (addr >> 14)
	{
	case 0: // 0000 - 3FFF
		break;
	case 1: // 4000 - 7FFF
		ram[addr + (((port7FFD & 0x08)) ? 0x18000 : 0x10000) ] = data;
		break;
	case 2: // 8000 - BFFF
		ram[addr] = data;
		break;
	case 3: // C000 - FFFF
		ram[(addr - 0xC000) + (0x4000 * (port7FFD & 7))] = data;
		break;
	}
}

uint8_t Specrtum128kBus::readPeripheral(uint16_t addr)
{
	uint8_t result = 0xFF;

	if ((addr & 0xFF) == 0xFE)
	{
		result = keyboard.getKey(addr >> 8);
		result = (audioIn > 0.3f) ? result | 0x40 : result & ~(0x40);
	}
	
	if (addr == 0x7FFD && !(port7FFD & 0x20))
	{
		result = port7FFD;
	}

	return result;
}

void Specrtum128kBus::writePeripheral(uint16_t addr, uint8_t data)
{
	if ((addr & 0xFF) == 0xFE)
	{
		speakerOut = (data & 0x18) ? 1.0f : 0.0f;
		video.borderColor = data & 0x07;
	}

	if (addr == 0x7FFD && !(port7FFD & 0x20))
	{
		port7FFD = data;
	}
}

void Specrtum128kBus::setSampleFrequency(uint32_t sampleRate)
{
	cpu.setSampleFrequency(sampleRate);
	video.setSampleFrequency(sampleRate);
}

void Specrtum128kBus::reset()
{
	cpu.reset();
	port7FFD = 0;
}

void Specrtum128kBus::clock()
{
	cpu.updateCpu();
	video.updateVideo();
	mixAudioInputs();
}

void Specrtum128kBus::mixAudioInputs()
{
	audioOut = ((speakerOut * 0.5) + audioIn) / 2;
}
