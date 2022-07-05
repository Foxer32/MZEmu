#include "Z80.h"

uint8_t Z80::ADDHLSS()
{
	uint16_t n = readRegisterPair2((currentOpCode & 0b00110000) >> 4);
	uint16_t sum = add16(readRegisterPair(RegisterPairs::HL), n);
	writeRegisterPair(RegisterPairs::HL, sum);

	return 0;
}

uint8_t Z80::ADCHLSS()
{
	uint16_t n = readRegisterPair2((currentOpCode & 0b00110000) >> 4);
	uint16_t sum = add16(readRegisterPair(RegisterPairs::HL), n, getFlag(Flags::C), true);
	writeRegisterPair(RegisterPairs::HL, sum);

	return 0;
}

uint8_t Z80::SBCHLSS()
{
	uint16_t n = readRegisterPair2((currentOpCode & 0b00110000) >> 4);
	uint16_t sum = sub16(readRegisterPair(RegisterPairs::HL), n, getFlag(Flags::C));
	writeRegisterPair(RegisterPairs::HL, sum);

	return 0;
}

uint8_t Z80::ADDIXPP()
{
	uint16_t n = readRegisterPair2((currentOpCode & 0b00110000) >> 4, IX, true);
	IX = add16(IX, n);

	return 0;
}

uint8_t Z80::ADDIYRR()
{
	uint16_t n = readRegisterPair2((currentOpCode & 0b00110000) >> 4, IY, true);
	IY = add16(IY, n);

	return 0;
}

uint8_t Z80::INCSS()
{
	uint8_t src = (currentOpCode & 0b00110000) >> 4;
	uint16_t n = readRegisterPair2(src);
	writeRegisterPair2(src, ++n);

	return 0;
}

uint8_t Z80::INCIX()
{
	IX++;

	return 0;
}

uint8_t Z80::INCIY()
{
	IY++;

	return 0;
}

uint8_t Z80::DECSS()
{
	uint8_t src = (currentOpCode & 0b00110000) >> 4;
	uint16_t n = readRegisterPair2(src);
	writeRegisterPair2(src, --n);

	return 0;
}

uint8_t Z80::DECIX()
{
	IX--;

	return 0;
}

uint8_t Z80::DECIY()
{
	IY--;

	return 0;
}

//Undocumented

uint8_t Z80::INCIXH()
{
	uint8_t hibyte = IX >> 8;
	uint8_t incVal = hibyte + 1;
	IX = (incVal << 8) | (IX & 0xFF);

	setIncFlags(hibyte, incVal);

	return 0;
}

uint8_t Z80::INCIXL()
{
	uint8_t lobyte = IX & 0xFF;
	uint8_t incVal = lobyte + 1;
	IX = (IX & 0xFF00) | incVal;

	setIncFlags(lobyte, incVal);

	return 0;
}

uint8_t Z80::INCIYH()
{
	uint8_t hibyte = IY >> 8;
	uint8_t incVal = hibyte + 1;
	IY = (incVal << 8) | (IY & 0xFF);

	setIncFlags(hibyte, incVal);

	return 0;
}

uint8_t Z80::INCIYL()
{
	uint8_t lobyte = IY & 0xFF;
	uint8_t incVal = lobyte + 1;
	IY = (IY & 0xFF00) | incVal;

	setIncFlags(lobyte, incVal);

	return 0;
}

uint8_t Z80::DECIXH()
{
	uint8_t hibyte = IX >> 8;
	uint8_t decVal = hibyte - 1;
	IX = (decVal << 8) | (IX & 0xFF);

	setDecFlags(hibyte, decVal);

	return 0;
}

uint8_t Z80::DECIYH()
{
	uint8_t hibyte = IY >> 8;
	uint8_t decVal = hibyte - 1;
	IY = (decVal << 8) | (IY & 0xFF);

	setDecFlags(hibyte, decVal);

	return 0;
}

uint8_t Z80::DECIXL()
{
	uint8_t lobyte = IX & 0xFF;
	uint8_t decVal = lobyte - 1;
	IX = (IX & 0xFF00) | decVal;

	setDecFlags(lobyte, decVal);

	return 0;
}

uint8_t Z80::DECIYL()
{
	uint8_t lobyte = IY & 0xFF;
	uint8_t decVal = lobyte - 1;
	IY = (IY & 0xFF00) | decVal;

	setDecFlags(lobyte, decVal);

	return 0;
}

uint8_t Z80::ADDAIXH()
{
	A = add8(A, IX >> 8);

	return 0;
}

uint8_t Z80::ADDAIXL()
{
	A = add8(A, IX & 0xFF);

	return 0;
}

uint8_t Z80::ADDAIYH()
{
	A = add8(A, IY >> 8);

	return 0;
}

uint8_t Z80::ADDAIYL()
{
	A = add8(A, IY & 0xFF);

	return 0;
}

uint8_t Z80::ADCAIXH()
{
	A = add8(A, IX >> 8, getFlag(Flags::C));

	return 0;
}

uint8_t Z80::ADCAIXL()
{
	A = add8(A, IX & 0xFF, getFlag(Flags::C));

	return 0;
}

uint8_t Z80::ADCAIYH()
{
	A = add8(A, IX >> 8, getFlag(Flags::C));

	return 0;
}

uint8_t Z80::ADCAIYL()
{
	A = add8(A, IX & 0xFF, getFlag(Flags::C));

	return 0;
}

uint8_t Z80::SUBAIXH()
{
	A = sub8(A, IX >> 8);

	return 0;
}

uint8_t Z80::SUBAIXL()
{
	A = sub8(A, IX & 0xFF);

	return 0;
}

uint8_t Z80::SUBAIYH()
{
	A = sub8(A, IY >> 8);

	return 0;
}

uint8_t Z80::SUBAIYL()
{
	A = sub8(A, IY & 0xFF);

	return 0;
}

uint8_t Z80::SBCAIXH()
{
	A = sub8(A, IX >> 8, getFlag(Flags::C));

	return 0;
}

uint8_t Z80::SBCAIXL()
{
	A = sub8(A, IX & 0xFF, getFlag(Flags::C));

	return 0;
}

uint8_t Z80::SBCAIYH()
{
	A = sub8(A, IX >> 8, getFlag(Flags::C));

	return 0;
}

uint8_t Z80::SBCAIYL()
{
	A = sub8(A, IX & 0xFF, getFlag(Flags::C));
	return 0;
}

uint8_t Z80::ANDIXH()
{
	A = andAB(A, IX >> 8);

	return 0;
}

uint8_t Z80::ANDIXL()
{
	A = andAB(A, IX & 0xFF);

	return 0;
}

uint8_t Z80::ANDIYH()
{
	A = andAB(A, IY >> 8);

	return 0;
}

uint8_t Z80::ANDIYL()
{
	A = andAB(A, IY & 0xFF);

	return 0;
}

uint8_t Z80::XORIXH()
{
	A = xorAB(A, IX >> 8);

	return 0;
}

uint8_t Z80::XORIXL()
{
	A = xorAB(A, IX & 0xFF);

	return 0;
}

uint8_t Z80::XORIYH()
{
	A = xorAB(A, IY >> 8);

	return 0;
}

uint8_t Z80::XORIYL()
{
	A = xorAB(A, IY & 0xFF);

	return 0;
}

uint8_t Z80::ORIXH()
{
	A = orAB(A, IX >> 8);

	return 0;
}

uint8_t Z80::ORIXL()
{
	A = orAB(A, IX & 0xFF);

	return 0;
}

uint8_t Z80::ORIYH()
{
	A = orAB(A, IY >> 8);

	return 0;
}

uint8_t Z80::ORIYL()
{
	A = orAB(A, IY & 0xFF);

	return 0;
}

uint8_t Z80::CPIXH()
{
	uint8_t n = IX >> 8;
	setComparsionFlags(n, A - n);

	return 0;
}

uint8_t Z80::CPIXL()
{
	uint8_t n = IX & 0xFF;
	setComparsionFlags(n, A - n);

	return 0;
}

uint8_t Z80::CPIYH()
{
	uint8_t n = IY >> 8;
	setComparsionFlags(n, A - n);

	return 0;
}

uint8_t Z80::CPIYL()
{
	uint8_t n = IY & 0xFF;
	setComparsionFlags(n, A - n);

	return 0;
}
