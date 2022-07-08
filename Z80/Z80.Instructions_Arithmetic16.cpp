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

uint8_t Z80::ADDIRPP()
{
	uint16_t n = readRegisterPair2((currentOpCode & 0b00110000) >> 4, IR, true);
	IR = add16(IR, n);
	saveIR();

	return 0;
}

uint8_t Z80::INCSS()
{
	uint8_t src = (currentOpCode & 0b00110000) >> 4;
	uint16_t n = readRegisterPair2(src);
	writeRegisterPair2(src, ++n);

	return 0;
}

uint8_t Z80::INCIR()
{
	IR++;
	saveIR();

	return 0;
}

uint8_t Z80::DECSS()
{
	uint8_t src = (currentOpCode & 0b00110000) >> 4;
	uint16_t n = readRegisterPair2(src);
	writeRegisterPair2(src, --n);

	return 0;
}

uint8_t Z80::DECIR()
{
	IR--;
	saveIR();

	return 0;
}

//Undocumented

uint8_t Z80::INCIRH()
{
	uint8_t hibyte = IR >> 8;
	uint8_t incVal = hibyte + 1;
	IR = (incVal << 8) | (IR & 0xFF);
	saveIR();

	setIncFlags(hibyte, incVal);

	return 0;
}

uint8_t Z80::INCIRL()
{
	uint8_t lobyte = IR & 0xFF;
	uint8_t incVal = lobyte + 1;
	IR = (IR & 0xFF00) | incVal;
	saveIR();

	setIncFlags(lobyte, incVal);

	return 0;
}

uint8_t Z80::DECIRH()
{
	uint8_t hibyte = IR >> 8;
	uint8_t decVal = hibyte - 1;
	IR = (decVal << 8) | (IR & 0xFF);
	saveIR();

	setDecFlags(hibyte, decVal);

	return 0;
}

uint8_t Z80::DECIRL()
{
	uint8_t lobyte = IR & 0xFF;
	uint8_t decVal = lobyte - 1;
	IR = (IR & 0xFF00) | decVal;
	saveIR();

	setDecFlags(lobyte, decVal);

	return 0;
}

uint8_t Z80::ADDAIRH()
{
	A = add8(A, IR >> 8);

	return 0;
}

uint8_t Z80::ADDAIRL()
{
	A = add8(A, IR & 0xFF);

	return 0;
}

uint8_t Z80::ADCAIRH()
{
	A = add8(A, IR >> 8, getFlag(Flags::C));

	return 0;
}

uint8_t Z80::ADCAIRL()
{
	A = add8(A, IR & 0xFF, getFlag(Flags::C));

	return 0;
}

uint8_t Z80::SUBAIRH()
{
	A = sub8(A, IR >> 8);

	return 0;
}

uint8_t Z80::SUBAIRL()
{
	A = sub8(A, IR & 0xFF);

	return 0;
}

uint8_t Z80::SBCAIRH()
{
	A = sub8(A, IR >> 8, getFlag(Flags::C));

	return 0;
}

uint8_t Z80::SBCAIRL()
{
	A = sub8(A, IR & 0xFF, getFlag(Flags::C));

	return 0;
}

uint8_t Z80::ANDIRH()
{
	A = andAB(A, IR >> 8);

	return 0;
}

uint8_t Z80::ANDIRL()
{
	A = andAB(A, IR & 0xFF);

	return 0;
}

uint8_t Z80::XORIRH()
{
	A = xorAB(A, IR >> 8);

	return 0;
}

uint8_t Z80::XORIRL()
{
	A = xorAB(A, IR & 0xFF);

	return 0;
}

uint8_t Z80::ORIRH()
{
	A = orAB(A, IR >> 8);

	return 0;
}

uint8_t Z80::ORIRL()
{
	A = orAB(A, IR & 0xFF);

	return 0;
}

uint8_t Z80::CPIRH()
{
	uint8_t n = IR >> 8;
	setComparsionFlags(n, A - n);

	return 0;
}

uint8_t Z80::CPIRL()
{
	uint8_t n = IR & 0xFF;
	setComparsionFlags(n, A - n);

	return 0;
}
