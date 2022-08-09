#pragma once
#include "SpectrumBus.h"
#include "SpectrumVideo.h"
#include "SpectrumKeyboard.h"

class Specrtum128kBus : public SpectrumBus
{
public:
	Specrtum128kBus();
	~Specrtum128kBus();

	virtual uint8_t readMemory(uint16_t addr) override;
	virtual void writeMemory(uint16_t addr, uint8_t data) override;
	virtual uint8_t readPeripheral(uint16_t addr) override;
	virtual void writePeripheral(uint16_t addr, uint8_t data) override;

	virtual void reset(bool hardReset = false) override;
	virtual void clock() override;

	uint8_t port7FFD = 0;
	uint8_t rom[32 * 1024];
	uint8_t ram[128 * 1024];

private:
	virtual void mixAudioInputs() override;

};
