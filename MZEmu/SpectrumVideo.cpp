#include "SpectrumVideo.h"
#include "IBus.h"

SpectrumVideo::SpectrumVideo()
{
	pal[0] = qRgb(0x00, 0x00, 0x00);
	pal[1] = qRgb(0x00, 0x00, 0xD7);
	pal[2] = qRgb(0xD7, 0x00, 0x00);
	pal[3] = qRgb(0xD7, 0x00, 0xD7);
	pal[4] = qRgb(0x00, 0xD7, 0x00);
	pal[5] = qRgb(0x00, 0xD7, 0xD7);
	pal[6] = qRgb(0xD7, 0xD7, 0x00);
	pal[7] = qRgb(0xD7, 0xD7, 0xD7);
	pal[8] = qRgb(0x00, 0x00, 0x00);
	pal[9] = qRgb(0x00, 0x00, 0xFF);
	pal[10] = qRgb(0xFF, 0x00, 0x00);
	pal[11] = qRgb(0xFF, 0x00, 0xFF);
	pal[12] = qRgb(0x00, 0xFF, 0x00);
	pal[13] = qRgb(0x00, 0xFF, 0xFF);
	pal[14] = qRgb(0xFF, 0xFF, 0x00);
	pal[15] = qRgb(0xFF, 0xFF, 0xFF);

	screenWidth = imageWidth + (borderSize * 2);
	screenHeight = imageHeight + (borderSize * 2);
}

SpectrumVideo::~SpectrumVideo()
{

}

void SpectrumVideo::setScreen(Screen* screen)
{
	this->screen = screen;
}

void SpectrumVideo::setSampleFrequency(uint32_t sampleRate)
{
	pixelUpdFreq = (float)(totalPixelsCount * imageUpdateFreq) / (float)sampleRate;
}

void SpectrumVideo::updateVideo()
{
	float tCount = lastPixel;
	while (tCount < pixelUpdFreq)
	{
		drawPixel(pixelCount++);

		if (pixelCount >= totalPixelsCount)
		{
			if (++frameCount >= 16)
			{
				videoFlashInvert = !videoFlashInvert;
				frameCount = 0;
			}

			screen->update();
			pixelCount = 0;
			bus->cpu.maskableInterrupt();
		}

		tCount++;
	}
	lastPixel = tCount - pixelUpdFreq;
}

void SpectrumVideo::drawPixel(uint32_t pixelNum)
{
	uint16_t x = pixelNum % columns;
	uint16_t y = pixelNum / columns;

	if (x >= (horizontalOffset - borderSize) && x < (horizontalOffset + imageWidth + borderSize) && y >= (vericalOffset - borderSize) && y < (vericalOffset + imageHeight + borderSize))
	{
		if (x < horizontalOffset || x >= (horizontalOffset + imageWidth) || y < vericalOffset || y >= (vericalOffset + imageHeight))
		{
			screen->screenBuffer.setPixel(x - horizontalOffset + borderSize, y - vericalOffset + borderSize, pal[borderColor]);
		}
		else
		{
			y -= vericalOffset;
			x -= horizontalOffset;

			uint16_t charY = 0x5800 + ((y >> 3) << 5);
			//uint8_t charX = x / (float)imageWidth * 32;
			uint8_t charX = x >> 3;

			uint16_t addr = 0x4000 | ((y & 0x07) << 8) | ((y & 0x38) << 2) | ((y & 0xC0) << 5) | charX & 0x1F;

			uint8_t att = bus->readMemory(charY + charX);
			uint8_t ink = att & 0x07;
			uint8_t paper = (att & 0x38) >> 3;
			if (att & 0x40)
			{
				ink |= 0x08;
				paper |= 0x08;
			}
			bool doFlash = (att & 0x80) && videoFlashInvert;

			uint8_t val = bus->readMemory(addr);

			y += borderSize;
			x += borderSize;

			screen->screenBuffer.setPixel(x, y, pal[(doFlash != (bool)(val & (0x80 >> (x % 8)))) ? ink : paper]);
		}
	}
}
