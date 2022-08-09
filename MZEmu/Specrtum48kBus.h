#pragma once
#include "SpectrumBus.h"
#include "SpectrumVideo.h"
#include "SpectrumKeyboard.h"

class Specrtum48kBus : public SpectrumBus
{
public:
	Specrtum48kBus();
	~Specrtum48kBus();

	virtual uint8_t readMemory(uint16_t addr) override;
	virtual void writeMemory(uint16_t addr, uint8_t data) override;
	virtual uint8_t readPeripheral(uint16_t addr) override;
	virtual void writePeripheral(uint16_t addr, uint8_t data) override;

	virtual void reset(bool hardReset = false) override;
	virtual void clock() override;

	uint8_t mem[64 * 1024];

private:
	virtual void mixAudioInputs() override;

};
