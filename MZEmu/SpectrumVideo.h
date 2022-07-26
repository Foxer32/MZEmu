#pragma once
#include "Device.h"
#include "Screen.h"

class SpectrumVideo : public Device
{
public:

	SpectrumVideo();
	~SpectrumVideo();

	void setScreen(Screen* screen);
	void setSampleFrequency(uint32_t sampleRate);
	void updateVideo();

	uint8_t borderColor = 0;

private:

	Screen* screen;

	unsigned int pal[16];

	float scanlineUpdFreq = 0;

	float lastLine = 0;
	uint16_t lineCount = 0;
	uint8_t frameCount = 0;
	bool videoFlashInvert = false;

	void drawLine(uint16_t lineNum);
	void fillBorder(uint16_t y, uint16_t start, uint16_t end);
};
