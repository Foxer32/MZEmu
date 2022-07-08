#include "Z80.h"

uint8_t Z80::JPNN()
{
	MEMPTR = PC = readMemoryNext2Bytes();
	resetQ();

	return 0;
}

uint8_t Z80::JPCCNN()
{
	absoluteAddress = readMemoryNext2Bytes();

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

uint8_t Z80::JPIR()
{
	PC = IR;
	resetQ();

	return 0;
}

uint8_t Z80::DJNZE()
{
	return JRif(--B);
}
