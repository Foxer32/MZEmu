#include "Specrtum48kBus.h"

Specrtum48kBus::Specrtum48kBus()
{
	cpu.connectBus(this);
	video.connectBus(this);

	for (auto& i : mem) i = 0x00;
	for (auto& i : keyMatrix) i = 0xFF;

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
	mem[addr] = data;
}

uint8_t Specrtum48kBus::readPeripheral(uint16_t addr)
{
	uint8_t result = 0xFF;

	if ((addr & 0xFF) == 0xFE)
	{
		uint8_t keyRow = addr >> 8;

		switch (keyRow)
		{
		case 0x7F:
			result &= keyMatrix[7];
			break;
		case 0xBF:
			result &= keyMatrix[6];
			break;
		case 0xDF:
			result &= keyMatrix[5];
			break;
		case 0xEF:
			result &= keyMatrix[4];
			break;
		case 0xF7:
			result &= keyMatrix[3];
			break;
		case 0xFB:
			result &= keyMatrix[2];
			break;
		case 0xFD:
			result &= keyMatrix[1];
			break;
		case 0xFE:
			result &= keyMatrix[0];
			break;
		}

		result = (audioIn > 0.3f) ? result | 0x40 : result & ~(0x40);
	}

	return result;
}

void Specrtum48kBus::writePeripheral(uint16_t addr, uint8_t data)
{
	if ((addr & 0xFF) == 0xFE)
	{
		audioOut = (data & 0x18) ? 1.0f : 0.0f;
		video.borderColor = data & 0x07;
	}
}

void Specrtum48kBus::setSampleFrequency(uint32_t sampleRate)
{
	cpuUpdFreq = (float)cpu.cpuFrequency / (float)sampleRate;
	video.setSampleFrequency(sampleRate);
}

void Specrtum48kBus::reset()
{
	cpu.reset();
}

void Specrtum48kBus::clock()
{
	float tCount = lastTCount;
	while (tCount < cpuUpdFreq)
		tCount += cpu.step();
	lastTCount = tCount - cpuUpdFreq;

	video.videoScan();
}
