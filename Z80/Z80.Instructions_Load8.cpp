#include "Z80.h"

uint8_t Z80::LDRR()
{
	uint8_t src = currentOpCode & 0b00000111;
	uint8_t dest = (currentOpCode & 0b00111000) >> 3;

	uint8_t srcReg = readFromRegister(src);
	writeToRgister(dest, srcReg);
	resetQ();

	return 0;
}

uint8_t Z80::LDRN()
{
	uint8_t dest = (currentOpCode & 0b00111000) >> 3;
	uint8_t n = readMemoryNext();

	writeToRgister(dest, n);
	resetQ();

	return 0;
}

uint8_t Z80::LDRHL()
{
	uint8_t dest = (currentOpCode & 0b00111000) >> 3;
	uint8_t n = readMemory(readRegisterPair(RegisterPairs::HL));

	writeToRgister(dest, n);
	resetQ();

	return 0;
}

uint8_t Z80::LDRIXD()
{
	uint8_t dest = (currentOpCode & 0b00111000) >> 3;
	int8_t d = readMemoryNext();

	absoluteAddress = IX + d;
	uint8_t n = readMemory(absoluteAddress);

	writeToRgister(dest, n);

	resetQ();
	MEMPTR = absoluteAddress;

	return 0;
}

uint8_t Z80::LDRIYD()
{
	uint8_t dest = (currentOpCode & 0b00111000) >> 3;
	int8_t d = readMemoryNext();

	absoluteAddress = IY + d;
	uint8_t n = readMemory(absoluteAddress);

	writeToRgister(dest, n);

	
	resetQ();
	MEMPTR = absoluteAddress;

	return 0;
}

uint8_t Z80::LDHLR()
{
	uint8_t src = currentOpCode & 0b00000111;
	uint8_t n = readFromRegister(src);

	writeMemory(readRegisterPair(RegisterPairs::HL), n);
	resetQ();

	return 0;
}

uint8_t Z80::LDIXDR()
{
	uint8_t src = currentOpCode & 0b00000111;
	uint8_t n = readFromRegister(src);
	int8_t d = readMemoryNext();
	absoluteAddress = IX + d;

	writeMemory(absoluteAddress,n);

	resetQ();
	MEMPTR = absoluteAddress;

	return 0;
}

uint8_t Z80::LDIYDR()
{
	uint8_t src = currentOpCode & 0b00000111;
	uint8_t n = readFromRegister(src);
	int8_t d = readMemoryNext();
	absoluteAddress = IY + d;

	writeMemory(absoluteAddress, n);

	resetQ();
	MEMPTR = absoluteAddress;

	return 0;
}

uint8_t Z80::LDHLN()
{
	uint8_t n = readMemoryNext();
	writeMemory(readRegisterPair(RegisterPairs::HL), n);

	resetQ();

	return 0;
}

uint8_t Z80::LDIXDN()
{
	int8_t d = readMemoryNext();
	uint8_t n = readMemoryNext();
	absoluteAddress = IX + d;

	writeMemory(absoluteAddress,n);

	resetQ();
	MEMPTR = absoluteAddress;

	return 0;
}

uint8_t Z80::LDIYDN()
{
	int8_t d = readMemoryNext();
	uint8_t n = readMemoryNext();
	uint16_t addr = IY + d;

	writeMemory(addr, n);

	resetQ();
	MEMPTR = addr;

	return 0;
}

uint8_t Z80::LDABC()
{
	absoluteAddress = readRegisterPair(RegisterPairs::BC);
	A = readMemory(absoluteAddress);

	resetQ();
	MEMPTR = ++absoluteAddress;

	return 0;
}

uint8_t Z80::LDADE()
{
	absoluteAddress = readRegisterPair(RegisterPairs::DE);
	A = readMemory(absoluteAddress);

	resetQ();
	MEMPTR = ++absoluteAddress;

	return 0;
}

uint8_t Z80::LDANN()
{
	uint8_t lobyte = readMemoryNext();
	uint8_t hibyte = readMemoryNext();
	absoluteAddress = (hibyte << 8) | lobyte;

	A = readMemory(absoluteAddress);

	resetQ();
	MEMPTR = ++absoluteAddress;

	return 0;
}

uint8_t Z80::LDBCA()
{
	absoluteAddress = readRegisterPair(RegisterPairs::BC);

	writeMemory(absoluteAddress, A);

	resetQ();
	MEMPTR = (A << 8) | (++absoluteAddress & 0xFF);

	return 0;
}

uint8_t Z80::LDDEA()
{
	absoluteAddress = readRegisterPair(RegisterPairs::DE);

	writeMemory(absoluteAddress, A);

	resetQ();
	MEMPTR = (A << 8) | (++absoluteAddress & 0xFF);

	return 0;
}

uint8_t Z80::LDNNA()
{
	uint8_t lobyte = readMemoryNext();
	uint8_t hibyte = readMemoryNext();
	absoluteAddress = (hibyte << 8) | lobyte;

	writeMemory(absoluteAddress,A);

	resetQ();
	MEMPTR = (A << 8) | (++absoluteAddress & 0xFF);

	return 0;
}

uint8_t Z80::LDAI()
{
	A = I;

	setFlag(Flags::S, I & 0x80);
	setFlag(Flags::Z, !I);
	setFlag(Flags::H, false);
	setFlag(Flags::P, IFF2);
	setFlag(Flags::N, false);	

	//TODO: if an interrupt occurs during this instruction, reset P/V
	setFlag(Flags::P, IFF2);
	setFlag(Flags::X, A & 0x08);
	setFlag(Flags::U, A & 0x20);
	setQ();

	return 0;
}

uint8_t Z80::LDAR()
{
	A = R;

	setFlag(Flags::S, R & 0x80);
	setFlag(Flags::Z, !R);
	setFlag(Flags::H, false);
	setFlag(Flags::P, IFF2);
	setFlag(Flags::N, false);

	//TODO: if an interrupt occurs during this instruction, reset P/V
	setFlag(Flags::P, IFF2);
	setFlag(Flags::X, A & 0x08);
	setFlag(Flags::U, A & 0x20);
	setQ();

	return 0;
}

uint8_t Z80::LDIA()
{
	I = A;
	resetQ();

	return 0;
}

uint8_t Z80::LDRA()
{
	R = A;
	resetQ();

	return 0;
}
