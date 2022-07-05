#include "Z80.h"

uint8_t Z80::LDDDNN()
{
	uint8_t dest = (currentOpCode & 0b00110000) >> 4;

	uint8_t lobyte = readMemoryNext();
	uint8_t hibyte = readMemoryNext();

	writeDD((hibyte << 8) | lobyte, dest);

	resetQ();

	return 0;
}

uint8_t Z80::LDIXNN()
{
	uint8_t lobyte = readMemoryNext();
	uint8_t hibyte = readMemoryNext();

	IX = (hibyte << 8) | lobyte;
	resetQ();

	return 0;
}

uint8_t Z80::LDIYNN()
{
	uint8_t lobyte = readMemoryNext();
	uint8_t hibyte = readMemoryNext();

	IY = (hibyte << 8) | lobyte;
	resetQ();

	return 0;
}

uint8_t Z80::LDHLNN()
{
	uint8_t lobyte = readMemoryNext();
	uint8_t hibyte = readMemoryNext();
	absoluteAddress = (hibyte << 8) | lobyte;

	L = readMemory(absoluteAddress++);
	H = readMemory(absoluteAddress);

	resetQ();
	MEMPTR = absoluteAddress;

	return 0;
}

uint8_t Z80::LDDDFNN()
{
	uint8_t dest = (currentOpCode & 0b00110000) >> 4;

	uint8_t lobyte = readMemoryNext();
	uint8_t hibyte = readMemoryNext();
	absoluteAddress = (hibyte << 8) | lobyte;

	uint8_t lobyte2 = readMemory(absoluteAddress++);
	uint8_t hibyte2 = readMemory(absoluteAddress);

	writeDD((hibyte2 << 8) | lobyte2, dest);

	resetQ();
	MEMPTR = absoluteAddress;

	return 0;
}

uint8_t Z80::LDIXFNN()
{
	uint8_t lobyte = readMemoryNext();
	uint8_t hibyte = readMemoryNext();
	absoluteAddress = (hibyte << 8) | lobyte;

	uint8_t lobyte2 = readMemory(absoluteAddress++);
	uint8_t hibyte2 = readMemory(absoluteAddress);

	IX = (hibyte2 << 8) | lobyte2;

	resetQ();
	MEMPTR = absoluteAddress;

	return 0;
}

uint8_t Z80::LDIYFNN()
{
	uint8_t lobyte = readMemoryNext();
	uint8_t hibyte = readMemoryNext();
	absoluteAddress = (hibyte << 8) | lobyte;

	uint8_t lobyte2 = readMemory(absoluteAddress++);
	uint8_t hibyte2 = readMemory(absoluteAddress);

	IY = (hibyte2 << 8) | lobyte2;

	resetQ();
	MEMPTR = absoluteAddress;

	return 0;
}

uint8_t Z80::LDNNHL()
{
	uint8_t lobyte = readMemoryNext();
	uint8_t hibyte = readMemoryNext();
	absoluteAddress = (hibyte << 8) | lobyte;

	writeMemory(absoluteAddress++,L);
	writeMemory(absoluteAddress, H);

	resetQ();
	MEMPTR = absoluteAddress;

	return 0;
}

uint8_t Z80::LDNNDD()
{
	uint8_t src = (currentOpCode & 0b00110000) >> 4;

	uint8_t lobyte = readMemoryNext();
	uint8_t hibyte = readMemoryNext();
	absoluteAddress = (hibyte << 8) | lobyte;

	uint16_t buff = 0x00;

	switch (src)
	{
	case 0:
		buff = readRegisterPair(RegisterPairs::BC);
	case 1:
		buff = readRegisterPair(RegisterPairs::DE);
	case 2:
		buff = readRegisterPair(RegisterPairs::HL);
	case 3:
		buff = SP;
	}

	writeMemory(absoluteAddress++, buff & 0xFF);
	writeMemory(absoluteAddress, buff >> 8);

	resetQ();
	MEMPTR = absoluteAddress;

	return 0;
}

uint8_t Z80::LDNNIX()
{
	uint8_t lobyte = readMemoryNext();
	uint8_t hibyte = readMemoryNext();
	absoluteAddress = (hibyte << 8) | lobyte;

	writeMemory(absoluteAddress++, IX & 0xFF);
	writeMemory(absoluteAddress, IX >> 8);

	resetQ();
	MEMPTR = absoluteAddress;

	return 0;
}

uint8_t Z80::LDNNIY()
{
	uint8_t lobyte = readMemoryNext();
	uint8_t hibyte = readMemoryNext();
	absoluteAddress = (hibyte << 8) | lobyte;

	writeMemory(absoluteAddress++, IY & 0xFF);
	writeMemory(absoluteAddress, IY >> 8);

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

uint8_t Z80::LDSPIX()
{
	SP = IX;
	resetQ();

	return 0;
}

uint8_t Z80::LDSPIY()
{
	SP = IY;
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

	SP--;
	writeMemory(SP, qq >> 8);
	SP--;
	writeMemory(SP, qq & 0xFF);
	
	resetQ();

	return 0;
}

uint8_t Z80::PUSHIX()
{
	SP--;
	writeMemory(SP, IX >> 8);
	SP--;
	writeMemory(SP, IX & 0xFF);

	resetQ();

	return 0;
}

uint8_t Z80::PUSHIY()
{
	SP--;
	writeMemory(SP, IY >> 8);
	SP--;
	writeMemory(SP, IY & 0xFF);

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

uint8_t Z80::POPIX()
{
	uint8_t hi = readMemory(SP++);
	uint8_t lo = readMemory(SP++);

	IX = (hi << 8) | lo;

	resetQ();

	return 0;
}

uint8_t Z80::POPIY()
{
	uint8_t hi = readMemory(SP++);
	uint8_t lo = readMemory(SP++);

	IX = (hi << 8) | lo;

	resetQ();

	return 0;
}

//Undocumented

uint8_t Z80::LDIXHN()
{
	IX = (readMemoryNext() << 8) | (IX & 0xFF);
	resetQ();

	return 0;
}

uint8_t Z80::LDIYHN()
{
	IY = (readMemoryNext() << 8) | (IY & 0xFF);
	resetQ();

	return 0;
}

uint8_t Z80::LDIXLN()
{
	IX = (IX & 0xFF00) | readMemoryNext();
	resetQ();

	return 0;
}

uint8_t Z80::LDIYLN()
{
	IY = (IY & 0xFF00) | readMemoryNext();
	resetQ();

	return 0;
}

uint8_t Z80::LDRIXH()
{
	writeToRgister((currentOpCode & 0b00111000) >> 3, IX >> 8);
	resetQ();

	return 0;
}

uint8_t Z80::LDRIYH()
{
	writeToRgister((currentOpCode & 0b00111000) >> 3, IY >> 8);
	resetQ();

	return 0;
}

uint8_t Z80::LDRIXL()
{
	writeToRgister((currentOpCode & 0b00111000) >> 3, IX & 0xFF);
	resetQ();

	return 0;
}

uint8_t Z80::LDRIYL()
{
	writeToRgister((currentOpCode & 0b00111000) >> 3, IY & 0xFF);
	resetQ();

	return 0;
}

uint8_t Z80::LDIXHR()
{
	IX = (readFromRegister(currentOpCode & 0b00000111) << 8) | (IX & 0xFF);
	resetQ();

	return 0;
}

uint8_t Z80::LDIYHR()
{
	IY = (readFromRegister(currentOpCode & 0b00000111) << 8) | (IY & 0xFF);
	resetQ();

	return 0;
}

uint8_t Z80::LDIXLR()
{
	IX = (IX & 0xFF00) | readFromRegister(currentOpCode & 0b00000111);
	resetQ();

	return 0;
}

uint8_t Z80::LDIYLR()
{
	IY = (IY & 0xFF00) | readFromRegister(currentOpCode & 0b00000111);
	resetQ();

	return 0;
}

uint8_t Z80::LDIXHIXH()
{
	resetQ();

	return 0;
}

uint8_t Z80::LDIXLIXL()
{
	resetQ();

	return 0;
}

uint8_t Z80::LDIXHIXL()
{
	IX = (IX << 8) | (IX & 0xFF);
	resetQ();

	return 0;
}

uint8_t Z80::LDIXLIXH()
{
	IX = (IX & 0xFF00) | (IX >> 8);
	resetQ();

	return 0;
}

uint8_t Z80::LDIYHIYH()
{
	resetQ();

	return 0;
}

uint8_t Z80::LDIYLIYL()
{
	resetQ();

	return 0;
}

uint8_t Z80::LDIYHIYL()
{
	IY = (IY << 8) | (IY & 0xFF);
	resetQ();

	return 0;
}

uint8_t Z80::LDIYLIYH()
{
	IY = (IY & 0xFF00) | (IY >> 8);
	resetQ();

	return 0;
}
