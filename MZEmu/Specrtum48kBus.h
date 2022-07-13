#pragma once
#include <cstdint>
#include <vector>

//#include "Z80.h"
#include "IBus.h"
#include "SpectrumVideo.h"


class Specrtum48kBus : public IBus
{
public:

	Specrtum48kBus();
	~Specrtum48kBus();

	virtual uint8_t readMemory(uint16_t addr) override;
	virtual void writeMemory(uint16_t addr,uint8_t data) override;
	virtual uint8_t readPeripheral(uint16_t addr) override;
	virtual void writePeripheral(uint16_t addr, uint8_t data) override;

	void setSampleFrequency(uint32_t sampleRate);

	void reset();
	void clock();

	SpectrumVideo video;

	uint8_t keyMatrix[8];

	bool audioOut = false;
	uint8_t mem[64 * 1024];
private:

	float cpuUpdFreq = 0;
	float lastTCount = 0;
};

