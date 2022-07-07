#include "Z80.h"

uint8_t Z80::CALLNN()
{
	uint8_t loByte = readMemoryNext();
	uint8_t hiByte = readMemoryNext();
	absoluteAddress = (hiByte << 8) | loByte;

	pushPC();

	PC = absoluteAddress;

	resetQ();
	MEMPTR = absoluteAddress;

	return 0;
}

uint8_t Z80::CALLCCNN()
{
	uint8_t loByte = readMemoryNext();
	uint8_t hiByte = readMemoryNext();
	absoluteAddress = (hiByte << 8) | loByte;

	uint8_t additionalTStates = 0;
	if (evaluateCC((currentOpCode & 0b00111000) >> 3))
	{
		pushPC();
		PC = absoluteAddress;
		additionalTStates = 7;
	}

	resetQ();
	MEMPTR = absoluteAddress;

	return additionalTStates;
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
	uint8_t additionalTStates = 0;
	if (evaluateCC((currentOpCode & 0b00111000) >> 3))
	{
		popPC();
		MEMPTR = PC;
		additionalTStates = 6;
	}

	resetQ();

	return 0;
}

uint8_t Z80::RETI()
{
	popPC();
	MEMPTR = PC;
	resetQ();
	//TODO: signal interrupting device that interrupt is complete
	return 0;
}

uint8_t Z80::RETN()
{
	IFF1 = IFF2;

	popPC();
	MEMPTR = PC;
	resetQ();

	return 0;
}

uint8_t Z80::RSTP()
{
	absoluteAddress = currentOpCode & 0b00111000;

	pushPC();
	PC = absoluteAddress;

	resetQ();
	MEMPTR = absoluteAddress;

	return 0;
}
