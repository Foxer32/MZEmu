#include "Z80.h"

uint8_t Z80::LDDDNN()
{
	uint8_t dest = (currentOpCode & 0b00110000) >> 4;
	uint16_t val = readMemoryNext2Bytes();

	writeDD(val, dest);

	resetQ();

	return 0;
}

uint8_t Z80::LDIRNN()
{
	IR = readMemoryNext2Bytes();
	saveIR();
	resetQ();

	return 0;
}

uint8_t Z80::LDHLNN()
{
	absoluteAddress = readMemoryNext2Bytes();

	L = readMemory(absoluteAddress++);
	H = readMemory(absoluteAddress);

	resetQ();
	MEMPTR = absoluteAddress;

	return 0;
}

uint8_t Z80::LDDDFNN()
{
	uint8_t dest = (currentOpCode & 0b00110000) >> 4;
	absoluteAddress = readMemoryNext2Bytes();

	uint8_t loByte = readMemory(absoluteAddress++);
	uint8_t hiByte = readMemory(absoluteAddress);

	writeDD((hiByte << 8) | loByte, dest);

	resetQ();
	MEMPTR = absoluteAddress;

	return 0;
}

uint8_t Z80::LDIRFNN()
{
	absoluteAddress = readMemoryNext2Bytes();

	uint8_t loByte = readMemory(absoluteAddress++);
	uint8_t hiByte = readMemory(absoluteAddress);

	IR = (hiByte << 8) | loByte;
	saveIR();

	resetQ();
	MEMPTR = absoluteAddress;

	return 0;
}

uint8_t Z80::LDNNHL()
{
	absoluteAddress = readMemoryNext2Bytes();

	writeMemory(absoluteAddress++,L);
	writeMemory(absoluteAddress, H);

	resetQ();
	MEMPTR = absoluteAddress;

	return 0;
}

uint8_t Z80::LDNNDD()
{
	uint8_t src = (currentOpCode & 0b00110000) >> 4;
	absoluteAddress = readMemoryNext2Bytes();

	uint16_t buff = 0x00;

	switch (src)
	{
	case 0:
		buff = readRegisterPair(RegisterPairs::BC);
		break;
	case 1:
		buff = readRegisterPair(RegisterPairs::DE);
		break;
	case 2:
		buff = readRegisterPair(RegisterPairs::HL);
		break;
	case 3:
		buff = SP;
		break;
	}

	writeMemory(absoluteAddress++, buff & 0xFF);
	writeMemory(absoluteAddress, buff >> 8);

	resetQ();
	MEMPTR = absoluteAddress;

	return 0;
}

uint8_t Z80::LDNNIR()
{
	absoluteAddress = readMemoryNext2Bytes();

	writeMemory(absoluteAddress++, IR & 0xFF);
	writeMemory(absoluteAddress, IR >> 8);

	resetQ();
	MEMPTR = absoluteAddress;

	return 0;
}

uint8_t Z80::LDSPHL()
{
	SP = readRegisterPair(RegisterPairs::HL);
	resetQ();

	return 0;
}

uint8_t Z80::LDSPIR()
{
	SP = IR;
	resetQ();

	return 0;
}

uint8_t Z80::PUSHQQ()
{
	uint16_t qq = 0x0000;

	switch ((currentOpCode & 0b00110000) >> 4)
	{
	case 0:
		qq = readRegisterPair(RegisterPairs::BC);
		break;
	case 1:
		qq = readRegisterPair(RegisterPairs::DE);
		break;
	case 2:
		qq = readRegisterPair(RegisterPairs::HL);
		break;
	case 3:
		qq = readRegisterPair(RegisterPairs::AF);
		break;
	}

	writeMemory(--SP, qq >> 8);
	writeMemory(--SP, qq & 0xFF);
	
	resetQ();

	return 0;
}

uint8_t Z80::PUSHIR()
{
	writeMemory(--SP, IR >> 8);
	writeMemory(--SP, IR & 0xFF);

	resetQ();

	return 0;
}

uint8_t Z80::POPQQ()
{
	uint8_t dest = (currentOpCode & 0b00110000) >> 4;

	uint8_t lo = readMemory(SP++);
	uint8_t hi = readMemory(SP++);

	switch (dest)
	{
	case 0:
		B = hi;
		C = lo;
		break;
	case 1:
		D = hi;
		E = lo;
		break;
	case 2:
		H = hi;
		L = lo;
		break;
	case 3:
		A = hi;
		F = lo;
		break;
	}

	resetQ();

	return 0;
}

uint8_t Z80::POPIR()
{
	uint8_t lo = readMemory(SP++);
	uint8_t hi = readMemory(SP++);

	IR = (hi << 8) | lo;
	saveIR();
	resetQ();

	return 0;
}

//Undocumented

uint8_t Z80::LDIRHN()
{
	IR = (readMemoryNext() << 8) | (IR & 0xFF);
	saveIR();
	resetQ();

	return 0;
}

uint8_t Z80::LDIRLN()
{
	IR = (IR & 0xFF00) | readMemoryNext();
	saveIR();
	resetQ();

	return 0;
}

uint8_t Z80::LDRIRH()
{
	writeToRgister((currentOpCode & 0b00111000) >> 3, IR >> 8);
	resetQ();

	return 0;
}

uint8_t Z80::LDRIRL()
{
	writeToRgister((currentOpCode & 0b00111000) >> 3, IR & 0xFF);
	resetQ();

	return 0;
}

uint8_t Z80::LDIRHR()
{
	IR = (readFromRegister(currentOpCode & 0b00000111) << 8) | (IR & 0xFF);
	saveIR();
	resetQ();

	return 0;
}

uint8_t Z80::LDIRLR()
{
	IR = (IR & 0xFF00) | readFromRegister(currentOpCode & 0b00000111);
	saveIR();
	resetQ();

	return 0;
}

uint8_t Z80::LDIRHIRH()
{
	resetQ();

	return 0;
}

uint8_t Z80::LDIRLIRL()
{
	resetQ();

	return 0;
}

uint8_t Z80::LDIRHIRL()
{
	IR = (IR << 8) | (IR & 0xFF);
	saveIR();
	resetQ();

	return 0;
}

uint8_t Z80::LDIRLIRH()
{
	IR = (IR & 0xFF00) | (IR >> 8);
	saveIR();
	resetQ();

	return 0;
}
