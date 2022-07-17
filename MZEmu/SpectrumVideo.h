#pragma once
#include "Device.h"
#include "olcPixelGameEngine.h"

class SpectrumVideo : public Device
{
public:

	SpectrumVideo();
	~SpectrumVideo();

	void setSampleFrequency(uint32_t sampleRate);
	void updateVideo();

	bool screenReady = false;
	olc::Sprite screenBuffer = olc::Sprite(320, 256);
	uint8_t borderColor = 0;

private:

	olc::Pixel pal[16];

	float scanlineUpdFreq = 0;

	float lastLine = 0;
	uint16_t lineCount = 0;
	uint8_t frameCount = 0;
	bool videoFlashInvert = false;

	void drawLine(uint16_t lineNum);
	void fillBorder(uint16_t y, uint16_t start, uint16_t end);
};
