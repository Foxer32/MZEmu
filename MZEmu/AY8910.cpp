#include "AY8910.h"

AY8910::AY8910()
{
	reset();

	generateEnvelope();

	for (int n = 0; n < 32; n++)
		for (int m = 0; m < 6; m++)
			vols[m][n] = (int)(((double)lion17AYTable[n / 2] * defaultLayout[1][m]) / 100);

	int maxL = (vols[0][31] + vols[2][31] + vols[3][31]);
	int maxR = (vols[1][31] + vols[3][31] + vols[5][31]);

	ampGlobal = ((maxL > maxR) ? maxL : maxR) / (0xFFFF / 2);
}

AY8910::~AY8910()
{

}

void AY8910::reset()
{
	for (auto& i : registers) i = 0x00;

	updateRegisters();
}

void AY8910::setFrequency(int frequency)
{
	deviceClockFrequency = frequency / 8;
}

int AY8910::step()
{
	int tmpVol = 0;
	int mixL = 0;
	int mixR = 0;

	if (++counterA >= toneA)
	{
		counterA = 0;
		bitA = !bitA;
	}

	if (++counterB >= toneB)
	{
		counterB = 0;
		bitB = !bitB;
	}

	if (++counterC >= toneC)
	{
		counterC = 0;
		bitC = !bitC;
	}

	if (++counterN >= (noise * 2))
	{
		counterN = 0;
		curSeed = (curSeed * 2 + 1) ^ (((curSeed >> 16) ^ (curSeed >> 13)) & 1);
		bitN = ((curSeed >> 16) & 1);
	}

	if (++counterE >= envFreq)
	{
		counterE = 0;
		if (++envPos > 127)
			envPos = 64;
	}

	if ((bitA && enableToneA) || (bitN && enableNoiseA))
	{
		tmpVol = envA ? envelope[envStyle][envPos] : volA * 2 + 1;
		mixL += vols[0][tmpVol];
		mixR += vols[1][tmpVol];
	}

	if ((bitB && enableToneB) || (bitN && enableNoiseB))
	{
		tmpVol = envB ? envelope[envStyle][envPos] : volB * 2 + 1;
		mixL += vols[2][tmpVol];
		mixR += vols[3][tmpVol];
	}

	if ((bitC && enableToneC) || (bitN && enableNoiseC))
	{
		tmpVol = envC ? envelope[envStyle][envPos] : volC * 2 + 1;
		mixL += vols[4][tmpVol];
		mixR += vols[5][tmpVol];
	}

	audioOut[0] = (mixL / ampGlobal) - (0xFFFF / 4);
	audioOut[1] = (mixR / ampGlobal) - (0xFFFF / 4);

	return 1;
}

uint8_t AY8910::readSelectedRegister()
{
	return registers[selectedRegister];
}

void AY8910::writeSelectedRegister(uint8_t val)
{
	registers[selectedRegister] = val;
	updateRegisters();
}

void AY8910::selectRegister(uint8_t reg)
{
	selectedRegister = reg & 0x0F;
}

void AY8910::updateRegisters()
{
	toneA = ((registers[1] & 0x0F) << 8) | registers[0];
	toneB = ((registers[3] & 0x0F) << 8) | registers[2];
	toneC = ((registers[5] & 0x0F) << 8) | registers[4];

	noise = (registers[6] & 0x1F);

	enableToneA = !(registers[7] & 0x01);
	enableToneB = !(registers[7] & 0x02);
	enableToneC = !(registers[7] & 0x04);

	enableNoiseA = !(registers[7] & 0x08);
	enableNoiseB = !(registers[7] & 0x10);
	enableNoiseC = !(registers[7] & 0x20);

	volA = registers[8] & 0x0F;
	volB = registers[9] & 0x0F;
	volC = registers[10] & 0x0F;
	envA = registers[8] & 0x10;
	envB = registers[9] & 0x10;
	envC = registers[10] & 0x10;
	envFreq = (registers[12] << 8) | registers[11];

	if (registers[13] != 0xFF)
	{
		envStyle = registers[13] & 0x0F;
		envPos = counterE = 0;
	}
}

void AY8910::generateEnvelope()
{
	bool hold;
	int dir;
	int vol;

	for (int env = 0; env < 16; env++)
	{
		hold = false;
		dir = (env & 4) ? 1 : -1;
		vol = (env & 4) ? -1 : 32;
		for (int pos = 0; pos < 128; pos++)
		{
			if (!hold)
			{
				vol += dir;
				if (vol < 0 || vol >= 32)
				{
					if (env & 8)
					{
						if (env & 2)
						{
							dir = -dir;
						}
						vol = (dir > 0) ? 0 : 31;
						if (env & 1)
						{
							hold = true;
							vol = (dir > 0) ? 31 : 0;
						}
					}
					else
					{
						vol = 0;
						hold = true;
					}
				}
			}
			envelope[env][pos] = vol;
		}
	}
}
