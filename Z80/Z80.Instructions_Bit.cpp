#include "Z80.h"

uint8_t Z80::BITBR()
{
	uint8_t n = readFromRegister(currentOpCode & 0b00000111);
	uint8_t bit = (currentOpCode & 0b00111000) >> 3;

	uint8_t res = n & (1 << bit);

	setFlag(Flags::Z, !res);
	setFlag(Flags::H, true);
	setFlag(Flags::N, false);

	setFlag(Flags::S, bit == 7 && res);
	setFlag(Flags::P, !res);
	setFlag(Flags::X, n & 0x08);
	setFlag(Flags::U, n & 0x20);

	setQ();

	return 0;
}

uint8_t Z80::BITBHL()
{
	uint8_t n = readMemory(readRegisterPair(RegisterPairs::HL));
	uint8_t bit = (currentOpCode & 0b00111000) >> 3;

	uint8_t res = n & (1 << bit);

	setFlag(Flags::Z, !res);
	setFlag(Flags::H, true);
	setFlag(Flags::N, false);

	setFlag(Flags::S, bit == 7 && res);
	setFlag(Flags::P, !res);
	setFlag(Flags::X, MEMPTR & 0x0800);
	setFlag(Flags::U, MEMPTR & 0x2000);

	setQ();

	return 0;
}

uint8_t Z80::BITBIXD()
{
	int8_t d = readMemory(PC - 2);
	absoluteAddress = IX + d;
	uint8_t n = readMemory(absoluteAddress);
	uint8_t bit = (currentOpCode & 0b00111000) >> 3;

	uint8_t res = n & (1 << bit);

	setFlag(Flags::Z, !res);
	setFlag(Flags::H, true);
	setFlag(Flags::N, false);

	MEMPTR = absoluteAddress;
	setFlag(Flags::S, bit == 7 && res);
	setFlag(Flags::P, !res);
	setFlag(Flags::X, MEMPTR & 0x0800);
	setFlag(Flags::U, MEMPTR & 0x2000);

	setQ();

	return 0;
}

uint8_t Z80::BITBIYD()
{
	int8_t d = readMemory(PC - 2);
	absoluteAddress = IY + d;
	uint8_t n = readMemory(absoluteAddress);
	uint8_t bit = (currentOpCode & 0b00111000) >> 3;

	uint8_t res = n & (1 << bit);

	setFlag(Flags::Z, !res);
	setFlag(Flags::H, true);
	setFlag(Flags::N, false);

	MEMPTR = absoluteAddress;
	setFlag(Flags::S, bit == 7 && res);
	setFlag(Flags::P, !res);
	setFlag(Flags::X, MEMPTR & 0x0800);
	setFlag(Flags::U, MEMPTR & 0x2000);

	setQ();

	return 0;
}

uint8_t Z80::SETBR()
{
	uint8_t src = currentOpCode & 0b00000111;
	uint8_t n = readFromRegister(src);
	uint8_t bit = (currentOpCode & 0b00111000) >> 3;

	n |= (1 << bit);

	writeToRgister(src, n);

	resetQ();

	return 0;
}

uint8_t Z80::SETBHL()
{
	uint8_t n = readMemory(readRegisterPair(RegisterPairs::HL));
	uint8_t bit = (currentOpCode & 0b00111000) >> 3;

	n |= (1 << bit);

	writeMemory(readRegisterPair(RegisterPairs::HL), n);

	resetQ();

	return 0;
}

uint8_t Z80::SETBIXD()
{
	int8_t d = readMemory(PC - 2);
	absoluteAddress = IX + d;
	uint8_t n = readMemory(absoluteAddress);
	uint8_t bit = (currentOpCode & 0b00111000) >> 3;

	n |= (1 << bit);

	writeMemory(absoluteAddress, n);

	uint8_t src = currentOpCode & 0b00000111;
	if (src != 6)
	{
		writeToRgister(src, n);
	}
	MEMPTR = absoluteAddress;

	resetQ();

	return 0;
}

uint8_t Z80::SETBIYD()
{
	int8_t d = readMemory(PC - 2);
	absoluteAddress = IY + d;
	uint8_t n = readMemory(absoluteAddress);
	uint8_t bit = (currentOpCode & 0b00111000) >> 3;

	n |= (1 << bit);

	writeMemory(absoluteAddress, n);

	uint8_t src = currentOpCode & 0b00000111;
	if (src != 6)
	{
		writeToRgister(src, n);
	}
	MEMPTR = absoluteAddress;

	resetQ();

	return 0;
}

uint8_t Z80::RESBR()
{
	uint8_t src = currentOpCode & 0b00000111;
	uint8_t n = readFromRegister(src);
	uint8_t bit = (currentOpCode & 0b00111000) >> 3;

	n &= ~(1 << bit);

	writeToRgister(src, n);

	resetQ();

	return 0;
}

uint8_t Z80::RESBHL()
{
	uint8_t n = readMemory(readRegisterPair(RegisterPairs::HL));
	uint8_t bit = (currentOpCode & 0b00111000) >> 3;

	n &= ~(1 << bit);

	writeMemory(readRegisterPair(RegisterPairs::HL), n);

	resetQ();

	return 0;
}

uint8_t Z80::RESBIXD()
{
	int8_t d = readMemory(PC - 2);
	absoluteAddress = IX + d;
	uint8_t n = readMemory(absoluteAddress);
	uint8_t bit = (currentOpCode & 0b00111000) >> 3;

	n &= ~(1 << bit);

	writeMemory(absoluteAddress, n);

	uint8_t src = currentOpCode & 0b00000111;
	if (src != 6)
	{
		writeToRgister(src, n);
	}
	MEMPTR = absoluteAddress;

	resetQ();

	return 0;
}

uint8_t Z80::RESBIYD()
{
	int8_t d = readMemory(PC - 2);
	absoluteAddress = IY + d;
	uint8_t n = readMemory(absoluteAddress);
	uint8_t bit = (currentOpCode & 0b00111000) >> 3;

	n &= ~(1 << bit);

	writeMemory(absoluteAddress, n);

	uint8_t src = currentOpCode & 0b00000111;
	if (src != 6)
	{
		writeToRgister(src, n);
	}
	MEMPTR = absoluteAddress;

	resetQ();

	return 0;
}
