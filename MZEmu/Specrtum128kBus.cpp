#include "Specrtum128kBus.h"
#include <fstream>

//#define MEASURE_EMULATION_SPEED

#ifdef MEASURE_EMULATION_SPEED
#include <QDebug>
#include <chrono>
using std::chrono::high_resolution_clock;
using std::chrono::duration_cast;
using std::chrono::duration;
int globalCounter = 0;
steady_clock::time_point t1, t2, t3, t4, t5;
float cpuTime, videoTime, ayTime, mixTime, totalTime;
#endif // MEASURE_EMULATION_SPEED

Specrtum128kBus::Specrtum128kBus()
{
	ay8910.setFrequency(1773400);

	for (auto& i : rom) i = 0x00;
	for (auto& i : ram) i = 0x00;

	std::ifstream ifs;
	ifs.open("roms/128.rom", std::ifstream::binary);
	if (ifs.is_open())
	{
		ifs.read((char*)rom, 32 * 1024);
		ifs.close();
	}
	/*
	std::ifstream ifs2;
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
	default:
		return 0x00;
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
	uint8_t result = video.attributePort;

	if ((addr & 0xFF) == 0x1F)
	{
		result = joystick.getJoystickPort();
	}

	if ((addr & 0xFF) == 0xFE)
	{
		result = keyboard.getKey(addr >> 8);
		result = (audioIn > 0.3f) ? result | 0x40 : result & ~(0x40);
	}
	
	if ((addr & 0xFF) == 0xFD && !(port7FFD & 0x20))
	{
		switch (addr >> 8)
		{
		case 0x7F:
			result = port7FFD;
			break;
		case 0xFF:
			result = ay8910.readSelectedRegister();
			break;
		}
	}

	return result;
}

void Specrtum128kBus::writePeripheral(uint16_t addr, uint8_t data)
{
	if ((addr & 0xFF) == 0xFE)
	{
		speakerOut = (((data & 0x18) >> 3) * 0.25f) - 1;
		video.borderColor = data & 0x07;
	}

	if ((addr & 0xFF) == 0xFD && !(port7FFD & 0x20))
	{
		switch (addr >> 8)
		{
		case 0x7F:
			port7FFD = data;
			break;
		case 0xBF:
			ay8910.writeSelectedRegister(data);
			break;
		case 0xFF:
			ay8910.selectRegister(data);
			break;
		}
	}
}

void Specrtum128kBus::reset(bool hardReset)
{
	cpu.reset(hardReset);
	port7FFD = 0;
	ay8910.reset();
}

void Specrtum128kBus::clock()
{
#ifdef MEASURE_EMULATION_SPEED
	t1 = high_resolution_clock::now();
	cpu.update();
	t2 = high_resolution_clock::now();
	video.update();
	t3 = high_resolution_clock::now();
	ay8910.update();
	t4 = high_resolution_clock::now();
	mixAudioInputs();
	t5 = high_resolution_clock::now();

	cpuTime = duration<float, std::micro>(t2 - t1).count();
	videoTime = duration<float, std::micro>(t3 - t2).count();
	ayTime = duration<float, std::micro>(t4 - t3).count();
	mixTime = duration<float, std::micro>(t5 - t4).count();
	totalTime = duration<float, std::micro>(t5 - t1).count();

	if (++globalCounter >= (getSampleFrequency() / 2))
	{
		globalCounter = 0;
		qInfo() << "--------------------------------";
		qInfo() << "cpu\t\t" << cpuTime << "us";
		qInfo() << "video\t" << videoTime << "us";
		qInfo() << "ay\t\t" << ayTime << "us";
		qInfo() << "mix\t\t" << mixTime << "us";
		qInfo() << "Total:\t" << totalTime << "us, System load: " << (totalTime / (1.0f / getSampleFrequency() * 1000000.0f)) * 100.0f << "%";
		qInfo() << "--------------------------------";
	}
#else
	cpu.update();
	video.update();
	ay8910.update();
	mixAudioInputs();
#endif // MEASURE_EMULATION_SPEED
}

void Specrtum128kBus::setSampleFrequency(uint32_t sampleRate)
{
	SpectrumBus::setSampleFrequency(sampleRate);
	ay8910.setSampleFrequency(sampleRate);
}

void Specrtum128kBus::mixAudioInputs()
{
	audioOut[0] = (speakerOut + audioIn + ay8910.audioOut[0]) / 3;
	audioOut[1] = (speakerOut + audioIn + ay8910.audioOut[1]) / 3;
}
