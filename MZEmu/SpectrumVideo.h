#pragma once
#include "Device.h"
#include "IClockable.h"
#include "GLScreen.h"

class SpectrumVideo : public Device, public IClockable
{
public:
	SpectrumVideo();
	~SpectrumVideo();

	void setScreen(GLScreen* screen);

	virtual int step() override;

	bool fastRender = false;

	uint8_t borderColor = 0;

	int screenWidth = 0;
	int screenHeight = 0;

	uint8_t attributePort = 0xFF;

private:
	const unsigned int imageWidth = 256;
	const unsigned int imageHeight = 192;
	const unsigned int imageUpdateFreq = 50;
	const unsigned int lines = 312;
	const unsigned int columns = 448;
	const unsigned int vericalOffset = 64;
	const unsigned int horizontalOffset = 144;
	const unsigned int totalPixelsCount = lines * columns;
	const unsigned int borderSize = 32;

	GLScreen* screen = nullptr;

	unsigned int pal[16];

	uint32_t pixelCount = 0;
	uint8_t frameCount = 0;
	bool videoFlashInvert = false;

	void drawPixel(uint32_t pixelNum);
	void drawAllScreen();

};
