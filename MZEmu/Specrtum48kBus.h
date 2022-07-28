#pragma once
#include <cstdint>
#include <vector>

#include "IBus.h"
#include "SpectrumVideo.h"
#include "SpectrumKeyboard.h"

class Specrtum48kBus : public IBus
{
public:

	Specrtum48kBus();
	~Specrtum48kBus();

	virtual uint8_t readMemory(uint16_t addr) override;
	virtual void writeMemory(uint16_t addr, uint8_t data) override;
	virtual uint8_t readPeripheral(uint16_t addr) override;
	virtual void writePeripheral(uint16_t addr, uint8_t data) override;

	void setSampleFrequency(uint32_t sampleRate);

	void reset(bool hardReset = false);
	void clock();

	SpectrumVideo video;
	SpectrumKeyboard keyboard;

	float audioOut = 0;
	float audioIn = 0;

	uint8_t mem[64 * 1024];
private:

	float speakerOut = 0;

	void mixAudioInputs();
};

