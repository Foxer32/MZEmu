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
	scanlineUpdFreq = (float)(312 * 50) / (float)sampleRate;
}

void SpectrumVideo::updateVideo()
{
	lastLine += scanlineUpdFreq;
	if (lastLine >= 312)
	{
		drawLine(lineCount++);
		if (lineCount >= 312)
		{
			if (++frameCount >= 16)
			{
				videoFlashInvert = !videoFlashInvert;
				frameCount = 0;
			}

			screen->update();
			lineCount = 0;
			bus->cpu.maskableInterrupt();
		}
		lastLine--;
	}
}

void SpectrumVideo::drawLine(uint16_t lineNum)
{
	if (lineNum >= 32 && lineNum < 288)
	{
		if (lineNum < 64 || lineNum >= 256)
		{
			fillBorder(lineNum - 32, 0, 320);
		}
		else
		{
			fillBorder(lineNum - 32, 0, 32);
			fillBorder(lineNum - 32, 288, 320);

			lineNum -= 64;

			uint16_t charY = 0x5800 + ((lineNum >> 3) << 5);
			for (uint8_t charX = 0; charX < 32; charX++)
			{
				uint16_t addr = 0x4000 | ((lineNum & 0x07) << 8) | ((lineNum & 0x38) << 2) | ((lineNum & 0xC0) << 5) | charX & 0x1F;

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

				for (uint8_t i = 0; i < 8; i++)
					screen->screenBuffer.setPixel(32 + (charX << 3) + i, lineNum + 32, pal[(doFlash != (bool)(val & (0x80 >> i))) ? ink : paper]);
			}
		}
	}
}

void SpectrumVideo::fillBorder(uint16_t y, uint16_t start, uint16_t end)
{
	while (start < end)
		screen->screenBuffer.setPixel(start++, y, pal[borderColor]);
}
