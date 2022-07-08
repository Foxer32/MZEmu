#include "Z80.h"

uint8_t Z80::CALLNN()
{
	absoluteAddress = readMemoryNext2Bytes();

	pushPC();
	MEMPTR = PC = absoluteAddress;
	resetQ();

	return 0;
}

uint8_t Z80::CALLCCNN()
{
	absoluteAddress = readMemoryNext2Bytes();

	if (evaluateCC((currentOpCode & 0b00111000) >> 3))
	{
		pushPC();
		MEMPTR = PC = absoluteAddress;
		return 7;
	}

	resetQ();
	MEMPTR = absoluteAddress;

	return 0;
}

uint8_t Z80::RET()
{
	popPC();
	MEMPTR = PC;
	resetQ();

	return 0;
}

uint8_t Z80::RETCC()
{
	if (evaluateCC((currentOpCode & 0b00111000) >> 3))
	{
		RET();
		return 6;
	}

	resetQ();

	return 0;
}

uint8_t Z80::RETI()
{
	RET();
	//TODO: signal interrupting device that interrupt is complete
	return 0;
}

uint8_t Z80::RETN()
{
	RET();
	IFF1 = IFF2;

	return 0;
}

uint8_t Z80::RSTP()
{
	pushPC();
	MEMPTR = PC = currentOpCode & 0b00111000;
	resetQ();

	return 0;
}
