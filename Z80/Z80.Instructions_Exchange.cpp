#include "Z80.h"

uint8_t Z80::EXDEHL()
{
	uint16_t temp = readRegisterPair(RegisterPairs::DE);

	writeRegisterPair(RegisterPairs::DE, readRegisterPair(RegisterPairs::HL));
	writeRegisterPair(RegisterPairs::HL, temp);

	resetQ();

	return 0;
}

uint8_t Z80::EXAFAF()
{
	uint16_t temp = readRegisterPair(RegisterPairs::AF);

	writeRegisterPair(RegisterPairs::AF, readRegisterPair(RegisterPairs::AF1));
	writeRegisterPair(RegisterPairs::AF1, temp);

	resetQ();

	return 0;
}

uint8_t Z80::EXX()
{
	uint16_t temp = readRegisterPair(RegisterPairs::BC);

	writeRegisterPair(RegisterPairs::BC, readRegisterPair(RegisterPairs::BC1));
	writeRegisterPair(RegisterPairs::BC1, temp);

	temp = readRegisterPair(RegisterPairs::DE);

	writeRegisterPair(RegisterPairs::DE, readRegisterPair(RegisterPairs::DE1));
	writeRegisterPair(RegisterPairs::DE1, temp);

	temp = readRegisterPair(RegisterPairs::HL);

	writeRegisterPair(RegisterPairs::HL, readRegisterPair(RegisterPairs::HL1));
	writeRegisterPair(RegisterPairs::HL1, temp);

	resetQ();

	return 0;
}

uint8_t Z80::EXSPHL()
{
	uint8_t tempLo = readMemory(SP);
	uint8_t tempHi = readMemory(SP + 1);

	writeMemory(SP, L);
	writeMemory(SP + 1, H);

	L = tempLo;
	H = tempHi;
	
	MEMPTR = (tempHi << 8) | tempLo;

	return 0;
}

uint8_t Z80::EXSPIX()
{
	uint8_t tempLo = readMemory(SP);
	uint8_t tempHi = readMemory(SP + 1);

	uint8_t ixL = IX & 0xFF;
	uint8_t ixH = IX >> 8;

	writeMemory(SP, ixL);
	writeMemory(SP + 1, ixH);

	IX = (tempHi << 8) | tempLo;

	MEMPTR = IX;

	return 0;
}

uint8_t Z80::EXSPIY()
{
	uint8_t tempLo = readMemory(SP);
	uint8_t tempHi = readMemory(SP + 1);

	uint8_t iyL = IY & 0xFF;
	uint8_t iyH = IY >> 8;

	writeMemory(SP, iyL);
	writeMemory(SP + 1, iyH);

	IY = (tempHi << 8) | tempLo;

	MEMPTR = IY;

	return 0;
}

uint8_t Z80::LDI()
{
	uint8_t n = readMemory(readRegisterPair(RegisterPairs::HL));
	writeMemory(readRegisterPair(RegisterPairs::DE), n);

	incRegisterPair(RegisterPairs::DE);
	incRegisterPair(RegisterPairs::HL);
	incRegisterPair(RegisterPairs::BC, -1);

	setFlag(Flags::H, false);
	setFlag(Flags::N, false);
	setFlag(Flags::P, readRegisterPair(RegisterPairs::BC));

	uint8_t temp = n + A;
	setFlag(Flags::X, temp & 0x08);
	setFlag(Flags::U, temp & 0x02);

	setQ();

	return 0;
}

uint8_t Z80::LDIR()
{
	LDI();

	uint8_t extraTStates = 0;
	if(readRegisterPair(RegisterPairs::BC))
	{
		PC -= 2;

		extraTStates = 5;
		MEMPTR = PC + 1;
	}

	return extraTStates;
}

uint8_t Z80::LDD()
{
	uint8_t n = readMemory(readRegisterPair(RegisterPairs::HL));
	writeMemory(readRegisterPair(RegisterPairs::DE), n);

	incRegisterPair(RegisterPairs::DE, -1);
	incRegisterPair(RegisterPairs::HL, -1);
	incRegisterPair(RegisterPairs::BC, -1);

	setFlag(Flags::H, false);
	setFlag(Flags::N, false);
	setFlag(Flags::P, readRegisterPair(RegisterPairs::BC));

	uint8_t temp = n + A;
	setFlag(Flags::X, temp & 0x08);
	setFlag(Flags::U, temp & 0x02);

	setQ();

	return 0;
}

uint8_t Z80::LDDR()
{
	LDD();

	uint8_t extraTStates = 0;
	if (readRegisterPair(RegisterPairs::BC))
	{
		PC -= 2;

		extraTStates = 5;
		MEMPTR = PC + 1;
	}

	return extraTStates;
}

uint8_t Z80::CPI()
{
	int8_t n = readMemory(readRegisterPair(RegisterPairs::HL));
	int8_t diff = A - n;

	incRegisterPair(RegisterPairs::HL);
	incRegisterPair(RegisterPairs::BC, -1);

	setFlag(Flags::S, diff & 0x80);
	setFlag(Flags::Z, !diff);
	setFlag(Flags::H,(A & 0x0F) < (n & 0x0F));
	setFlag(Flags::P, readRegisterPair(RegisterPairs::BC));
	setFlag(Flags::N, true);

	uint8_t temp = diff - getFlag(Flags::H);
	setFlag(Flags::X, temp & 0x08);
	setFlag(Flags::U, temp & 0x02);

	MEMPTR++;

	setQ();

	return 0;
}

uint8_t Z80::CPIR()
{
	CPI();

	uint8_t extraTStates = 0;
	if (readRegisterPair(RegisterPairs::BC) && !getFlag(Flags::Z))
	{
		PC -= 2;

		extraTStates = 5;
		MEMPTR = PC + 1;
	}

	return extraTStates;
}

uint8_t Z80::CPD()
{
	int8_t n = readMemory(readRegisterPair(RegisterPairs::HL));
	int8_t diff = A - n;

	incRegisterPair(RegisterPairs::HL, -1);
	incRegisterPair(RegisterPairs::BC, -1);

	setFlag(Flags::S, diff & 0x80);
	setFlag(Flags::Z, !diff);
	setFlag(Flags::H, (A & 0x0F) < (n & 0x0F));
	setFlag(Flags::P, readRegisterPair(RegisterPairs::BC));
	setFlag(Flags::N, true);

	uint8_t temp = diff - getFlag(Flags::H);
	setFlag(Flags::X, temp & 0x08);
	setFlag(Flags::U, temp & 0x02);

	MEMPTR--;

	setQ();

	return 0;
}

uint8_t Z80::CPDR()
{
	CPD();

	uint8_t extraTStates = 0;
	if (readRegisterPair(RegisterPairs::BC) && !getFlag(Flags::Z))
	{
		PC -= 2;

		extraTStates = 5;
		MEMPTR = PC + 1;
	}

	return extraTStates;
}
