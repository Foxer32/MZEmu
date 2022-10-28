#pragma once
#include <cstdint>
#include "IClockable.h"

class AY8910 : public IClockable
{
public:
	AY8910();
	~AY8910();

	void reset();

	void setFrequency(int frequency);
	virtual int step() override;

	uint8_t readSelectedRegister();
	void writeSelectedRegister(uint8_t val);
	void selectRegister(uint8_t reg);

	int16_t audioOut[2] = { 0,0 };

private:
	void updateRegisters();
	void generateEnvelope();

	uint8_t selectedRegister = 0;
	uint8_t registers[16];

	int ampGlobal = 0;

	int envelope[16][128];
	int vols[6][32];

	const int lion17AYTable[16] =
	{ 0, 513, 828, 1239, 1923, 3238, 4926, 9110,
	  10344, 17876, 24682, 30442, 38844, 47270, 56402, 65535 };

	const int defaultLayout[7][6] = {
		/* A_l, A_r,  B_l, B_r,  C_l, C_r */

		{100, 100, 100, 100, 100, 100},	// MONO
		{100, 33, 70, 70, 33, 100},	   // ABC
		{100, 33, 33, 100, 70, 70},	   // ACB
		{70, 70, 100, 33, 33, 100},	   // BAC
		{33, 100, 100, 33, 70, 70},	   // BCA
		{70, 70, 33, 100, 100, 33},	   // CAB
		{33, 100, 70, 70, 100, 33}   // CBA
	};

	uint16_t toneA = 0;
	uint16_t toneB = 0;
	uint16_t toneC = 0;
	uint8_t noise = 0;
	bool enableToneA = false;
	bool enableToneB = false;
	bool enableToneC = false;
	bool enableNoiseA = false;
	bool enableNoiseB = false;
	bool enableNoiseC = false;
	uint8_t volA = 0;
	uint8_t volB = 0;
	uint8_t volC = 0;
	bool envA = 0;
	bool envB = 0;
	bool envC = 0;
	uint16_t envFreq = 0;
	uint8_t envStyle = 0;

	bool bitA = false;
	bool bitB = false;
	bool bitC = false;
	bool bitN = false;

	uint16_t counterA = 0;
	uint16_t counterB = 0;
	uint16_t counterC = 0;
	uint16_t counterN = 0;
	uint16_t counterE = 0;

	uint8_t envPos = 0;
	uint32_t curSeed = 0xFFFF;

};
