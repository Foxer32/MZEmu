#include "Z80.h"

uint8_t Z80::JPNN()
{
	uint8_t loByte = readMemoryNext();
	uint8_t hiByte = readMemoryNext();
	absoluteAddress = (hiByte << 8) | loByte;

	PC = absoluteAddress;

	resetQ();
	MEMPTR = absoluteAddress;

	return 0;
}

uint8_t Z80::JPCCNN()
{
	uint8_t loByte = readMemoryNext();
	uint8_t hiByte = readMemoryNext();
	absoluteAddress = (hiByte << 8) | loByte;

	if (evaluateCC((currentOpCode & 0b00111000) >> 3))
	{
		PC = absoluteAddress;
	}

	resetQ();
	MEMPTR = absoluteAddress;

	return 0;
}

uint8_t Z80::JRE()
{
	int8_t e = readMemoryNext();
	PC += e;

	resetQ();
	MEMPTR = PC;

	return 0;
}

uint8_t Z80::JRCE()
{
	return JRif(getFlag(Flags::C));
}

uint8_t Z80::JRNCE()
{
	return JRif(!getFlag(Flags::C));
}

uint8_t Z80::JRZE()
{
	return JRif(getFlag(Flags::Z));
}

uint8_t Z80::JRNZE()
{
	return JRif(!getFlag(Flags::Z));
}

uint8_t Z80::JPHL()
{
	PC = readRegisterPair(RegisterPairs::HL);

	resetQ();
	return 0;
}

uint8_t Z80::JPIX()
{
	PC = IX;

	resetQ();
	return 0;
}

uint8_t Z80::JPIY()
{
	PC = IY;

	resetQ();
	return 0;
}

uint8_t Z80::DJNZE()
{
	B--;
	return JRif(B);
}
