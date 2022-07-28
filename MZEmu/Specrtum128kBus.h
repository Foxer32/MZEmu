#pragma once
#include <cstdint>
#include <vector>

#include "IBus.h"
#include "SpectrumVideo.h"
#include "SpectrumKeyboard.h"

class Specrtum128kBus : public IBus
{
public:
	Specrtum128kBus();
	~Specrtum128kBus();

	virtual uint8_t readMemory(uint16_t addr) override;
	virtual void writeMemory(uint16_t addr, uint8_t data) override;
	virtual uint8_t readPeripheral(uint16_t addr) override;
	virtual void writePeripheral(uint16_t addr, uint8_t data) override;

	void setSampleFrequency(uint32_t sampleRate);

	void reset(bool hardReset = false);
	void clock();

	void setPausedStatus(bool status);
	void setMaxSpeedStatus(bool status);

	SpectrumVideo video;
	SpectrumKeyboard keyboard;

	float audioOut = 0;
	float audioIn = 0;

	uint8_t port7FFD = 0;
	uint8_t rom[32 * 1024];
	uint8_t ram[128 * 1024];

private:
	void mixAudioInputs();

	bool pausedStatus = false;
	bool maxSpeedStatus = false;

	float speakerOut = 0;
};
