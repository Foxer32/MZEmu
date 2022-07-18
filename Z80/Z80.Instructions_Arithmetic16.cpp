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
	IR = (incN(IR >> 8) << 8) | (IR & 0xFF);
	saveIR();

	return 0;
}

uint8_t Z80::INCIRL()
{
	IR = (IR & 0xFF00) | incN(IR & 0xFF);
	saveIR();

	return 0;
}

uint8_t Z80::DECIRH()
{
	IR = (decN(IR >> 8) << 8) | (IR & 0xFF);
	saveIR();

	return 0;
}

uint8_t Z80::DECIRL()
{
	IR = (IR & 0xFF00) | decN(IR & 0xFF);
	saveIR();

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
	andAN(IR >> 8);

	return 0;
}

uint8_t Z80::ANDIRL()
{
	andAN(IR & 0xFF);

	return 0;
}

uint8_t Z80::XORIRH()
{
	xorAN(IR >> 8);

	return 0;
}

uint8_t Z80::XORIRL()
{
	xorAN(IR & 0xFF);

	return 0;
}

uint8_t Z80::ORIRH()
{
	orAN(IR >> 8);

	return 0;
}

uint8_t Z80::ORIRL()
{
	orAN(IR & 0xFF);

	return 0;
}

uint8_t Z80::CPIRH()
{
	compareAN(IR >> 8);

	return 0;
}

uint8_t Z80::CPIRL()
{
	compareAN(IR & 0xFF);

	return 0;
}
