#include "Z80.h"

uint8_t Z80::RLCA()
{
	A = (A << 1) | (A >> 7);

	setFlag(Flags::H, false);
	setFlag(Flags::N, false);
	setFlag(Flags::C, A & 1);

	setFlag(Flags::X, A & 0x08);
	setFlag(Flags::U, A & 0x20);
	setQ();

	return 0;
}

uint8_t Z80::RLA()
{
	bool newC = A & 0x80;
	A = (A << 1) | getFlag(Flags::C);

	setFlag(Flags::H, false);
	setFlag(Flags::N, false);
	setFlag(Flags::C, newC);

	setFlag(Flags::X, A & 0x08);
	setFlag(Flags::U, A & 0x20);
	setQ();

	return 0;
}

uint8_t Z80::RRCA()
{
	A = (A >> 1) | (A << 7);

	setFlag(Flags::H, false);
	setFlag(Flags::N, false);
	setFlag(Flags::C, A & 0x80);

	setFlag(Flags::X, A & 0x08);
	setFlag(Flags::U, A & 0x20);
	setQ();

	return 0;
}

uint8_t Z80::RRA()
{
	bool newC = A & 1;
	A = (A >> 1) | (getFlag(Flags::C) << 7);

	setFlag(Flags::H, false);
	setFlag(Flags::N, false);
	setFlag(Flags::C, newC);

	setFlag(Flags::X, A & 0x08);
	setFlag(Flags::U, A & 0x20);
	setQ();

	return 0;
}

uint8_t Z80::RLCR()
{
	uint8_t src = currentOpCode & 0b00000111;
	uint8_t n = readFromRegister(src);

	n = (n << 1) | (n >> 7);

	writeToRgister(src, n);

	setFlag(Flags::C, n & 1);
	setRotateFlags(n);

	return 0;
}

uint8_t Z80::RLCHL()
{
	uint8_t n = readMemory(readRegisterPair(RegisterPairs::HL));

	n = (n << 1) | (n >> 7);

	writeMemory(readRegisterPair(RegisterPairs::HL), n);

	setFlag(Flags::C, n & 1);
	setRotateFlags(n);

	return 0;
}

uint8_t Z80::RLCIXD()
{
	int8_t d = readMemory(PC - 2);
	absoluteAddress = IX + d;

	uint8_t n = readMemory(absoluteAddress);

	n = (n << 1) | (n >> 7);

	writeMemory(absoluteAddress, n);

	setFlag(Flags::C, n & 1);
	setRotateFlags(n);

	uint8_t dest = currentOpCode & 0b00000111;
	if (dest != 6)
	{
		writeToRgister(dest,n);
	}
	MEMPTR = absoluteAddress;

	return 0;
}

uint8_t Z80::RLCIYD()
{
	int8_t d = readMemory(PC - 2);
	absoluteAddress = IY + d;

	uint8_t n = readMemory(absoluteAddress);

	n = (n << 1) | (n >> 7);

	writeMemory(absoluteAddress, n);

	setFlag(Flags::C, n & 1);
	setRotateFlags(n);

	uint8_t dest = currentOpCode & 0b00000111;
	if (dest != 6)
	{
		writeToRgister(dest, n);
	}
	MEMPTR = absoluteAddress;

	return 0;
}

uint8_t Z80::RLR()
{
	uint8_t src = currentOpCode & 0b00000111;
	uint8_t n = readFromRegister(src);

	bool newC = n & 0x80;
	n = (n << 1) | getFlag(Flags::C);

	writeToRgister(src, n);

	setFlag(Flags::C, newC);
	setRotateFlags(n);

	return 0;
}

uint8_t Z80::RLHL()
{
	uint8_t n = readMemory(readRegisterPair(RegisterPairs::HL));

	bool newC = n & 0x80;
	n = (n << 1) | getFlag(Flags::C);

	writeMemory(readRegisterPair(RegisterPairs::HL), n);

	setFlag(Flags::C, newC);
	setRotateFlags(n);

	return 0;
}

uint8_t Z80::RLIXD()
{
	int8_t d = readMemory(PC - 2);
	absoluteAddress = IX + d;

	uint8_t n = readMemory(absoluteAddress);

	bool newC = n & 0x80;
	n = (n << 1) | getFlag(Flags::C);

	writeMemory(absoluteAddress, n);

	setFlag(Flags::C, newC);
	setRotateFlags(n);

	uint8_t dest = currentOpCode & 0b00000111;
	if (dest != 6)
	{
		writeToRgister(dest, n);
	}
	MEMPTR = absoluteAddress;

	return 0;
}

uint8_t Z80::RLIYD()
{
	int8_t d = readMemory(PC - 2);
	absoluteAddress = IY + d;

	uint8_t n = readMemory(absoluteAddress);

	bool newC = n & 0x80;
	n = (n << 1) | getFlag(Flags::C);

	writeMemory(absoluteAddress, n);

	setFlag(Flags::C, newC);
	setRotateFlags(n);

	uint8_t dest = currentOpCode & 0b00000111;
	if (dest != 6)
	{
		writeToRgister(dest, n);
	}
	MEMPTR = absoluteAddress;

	return 0;
}

uint8_t Z80::RRCR()
{
	uint8_t src = currentOpCode & 0b00000111;
	uint8_t n = readFromRegister(src);

	n = (n >> 1) | (n << 7);

	writeToRgister(src, n);

	setFlag(Flags::C, n & 0x80);
	setRotateFlags(n);

	return 0;
}

uint8_t Z80::RRCHL()
{
	uint8_t n = readMemory(readRegisterPair(RegisterPairs::HL));

	n = (n >> 1) | (n << 7);

	writeMemory(readRegisterPair(RegisterPairs::HL), n);

	setFlag(Flags::C, n & 0x80);
	setRotateFlags(n);

	return 0;
}

uint8_t Z80::RRCIXD()
{
	int8_t d = readMemory(PC - 2);
	absoluteAddress = IX + d;

	uint8_t n = readMemory(absoluteAddress);

	n = (n >> 1) | (n << 7);

	writeMemory(absoluteAddress, n);

	setFlag(Flags::C, n & 0x80);
	setRotateFlags(n);

	uint8_t dest = currentOpCode & 0b00000111;
	if (dest != 6)
	{
		writeToRgister(dest, n);
	}
	MEMPTR = absoluteAddress;

	return 0;
}

uint8_t Z80::RRCIYD()
{
	int8_t d = readMemory(PC - 2);
	absoluteAddress = IY + d;

	uint8_t n = readMemory(absoluteAddress);

	n = (n >> 1) | (n << 7);

	writeMemory(absoluteAddress, n);

	setFlag(Flags::C, n & 0x80);
	setRotateFlags(n);

	uint8_t dest = currentOpCode & 0b00000111;
	if (dest != 6)
	{
		writeToRgister(dest, n);
	}
	MEMPTR = absoluteAddress;

	return 0;
}

uint8_t Z80::RRR()
{
	uint8_t src = currentOpCode & 0b00000111;
	uint8_t n = readFromRegister(src);

	bool newC = n & 1;
	n = (n >> 1) | (getFlag(Flags::C) << 7);

	writeToRgister(src, n);

	setFlag(Flags::C, newC);
	setRotateFlags(n);

	return 0;
}

uint8_t Z80::RRHL()
{
	uint8_t n = readMemory(readRegisterPair(RegisterPairs::HL));

	bool newC = n & 1;
	n = (n >> 1) | (getFlag(Flags::C) << 7);

	writeMemory(readRegisterPair(RegisterPairs::HL), n);

	setFlag(Flags::C, newC);
	setRotateFlags(n);

	return 0;
}

uint8_t Z80::RRIXD()
{
	int8_t d = readMemory(PC - 2);
	absoluteAddress = IY + d;

	uint8_t n = readMemory(absoluteAddress);

	bool newC = n & 1;
	n = (n >> 1) | (getFlag(Flags::C) << 7);

	writeMemory(absoluteAddress, n);

	setFlag(Flags::C, newC);
	setRotateFlags(n);

	uint8_t dest = currentOpCode & 0b00000111;
	if (dest != 6)
	{
		writeToRgister(dest, n);
	}
	MEMPTR = absoluteAddress;

	return 0;
}

uint8_t Z80::RRIYD()
{
	int8_t d = readMemory(PC - 2);
	absoluteAddress = IY + d;

	uint8_t n = readMemory(absoluteAddress);

	bool newC = n & 1;
	n = (n >> 1) | (getFlag(Flags::C) << 7);

	writeMemory(absoluteAddress, n);

	setFlag(Flags::C, newC);
	setRotateFlags(n);

	uint8_t dest = currentOpCode & 0b00000111;
	if (dest != 6)
	{
		writeToRgister(dest, n);
	}
	MEMPTR = absoluteAddress;

	return 0;
}

uint8_t Z80::SLAR()
{
	uint8_t src = currentOpCode & 0b00000111;
	uint8_t n = readFromRegister(src);

	bool newC = n & 0x80;
	n <<= 1;

	writeToRgister(src, n);

	setFlag(Flags::C, newC);
	setRotateFlags(n);

	return 0;
}

uint8_t Z80::SLAHL()
{
	uint8_t n = readMemory(readRegisterPair(RegisterPairs::HL));

	bool newC = n & 0x80;
	n <<= 1;

	writeMemory(readRegisterPair(RegisterPairs::HL), n);

	setFlag(Flags::C, newC);
	setRotateFlags(n);

	return 0;
}

uint8_t Z80::SLAIXD()
{
	int8_t d = readMemory(PC - 2);
	absoluteAddress = IX + d;

	uint8_t n = readMemory(absoluteAddress);

	bool newC = n & 0x80;
	n <<= 1;

	writeMemory(absoluteAddress, n);

	setFlag(Flags::C, newC);
	setRotateFlags(n);

	uint8_t dest = currentOpCode & 0b00000111;
	if (dest != 6)
	{
		writeToRgister(dest, n);
	}
	MEMPTR = absoluteAddress;

	return 0;
}

uint8_t Z80::SLAIYD()
{
	int8_t d = readMemory(PC - 2);
	absoluteAddress = IY + d;

	uint8_t n = readMemory(absoluteAddress);

	bool newC = n & 0x80;
	n <<= 1;

	writeMemory(absoluteAddress, n);

	setFlag(Flags::C, newC);
	setRotateFlags(n);

	uint8_t dest = currentOpCode & 0b00000111;
	if (dest != 6)
	{
		writeToRgister(dest, n);
	}
	MEMPTR = absoluteAddress;

	return 0;
}

uint8_t Z80::SRAR()
{
	uint8_t src = currentOpCode & 0b00000111;
	uint8_t n = readFromRegister(src);

	bool newC = n & 1;
	n = (n >> 1) | (n & 0x80);

	writeToRgister(src, n);

	setFlag(Flags::C, newC);
	setRotateFlags(n);

	return 0;
}

uint8_t Z80::SRAHL()
{
	uint8_t n = readMemory(readRegisterPair(RegisterPairs::HL));

	bool newC = n & 1;
	n = (n >> 1) | (n & 0x80);

	writeMemory(readRegisterPair(RegisterPairs::HL), n);

	setFlag(Flags::C, newC);
	setRotateFlags(n);

	return 0;
}

uint8_t Z80::SRAIXD()
{
	int8_t d = readMemory(PC - 2);
	absoluteAddress = IY + d;

	uint8_t n = readMemory(absoluteAddress);

	bool newC = n & 1;
	n = (n >> 1) | (n & 0x80);

	writeMemory(absoluteAddress, n);

	setFlag(Flags::C, newC);
	setRotateFlags(n);

	uint8_t dest = currentOpCode & 0b00000111;
	if (dest != 6)
	{
		writeToRgister(dest, n);
	}
	MEMPTR = absoluteAddress;

	return 0;
}

uint8_t Z80::SRAIYD()
{
	int8_t d = readMemory(PC - 2);
	absoluteAddress = IY + d;

	uint8_t n = readMemory(absoluteAddress);

	bool newC = n & 1;
	n = (n >> 1) | (n & 0x80);

	writeMemory(absoluteAddress, n);

	setFlag(Flags::C, newC);
	setRotateFlags(n);

	uint8_t dest = currentOpCode & 0b00000111;
	if (dest != 6)
	{
		writeToRgister(dest, n);
	}
	MEMPTR = absoluteAddress;

	return 0;
}

uint8_t Z80::SRLR()
{
	uint8_t src = currentOpCode & 0b00000111;
	uint8_t n = readFromRegister(src);

	bool newC = n & 1;
	n >>= 1;

	writeToRgister(src, n);

	setFlag(Flags::C, newC);
	setRotateFlags(n, true);

	return 0;
}

uint8_t Z80::SRLHL()
{
	uint8_t n = readMemory(readRegisterPair(RegisterPairs::HL));

	bool newC = n & 1;
	n >>= 1;

	writeMemory(readRegisterPair(RegisterPairs::HL), n);

	setFlag(Flags::C, newC);
	setRotateFlags(n, true);

	return 0;
}

uint8_t Z80::SRLIXD()
{
	int8_t d = readMemory(PC - 2);
	absoluteAddress = IX + d;

	uint8_t n = readMemory(absoluteAddress);

	bool newC = n & 1;
	n >>= 1;

	writeMemory(absoluteAddress, n);

	setFlag(Flags::C, newC);
	setRotateFlags(n, true);

	uint8_t dest = currentOpCode & 0b00000111;
	if (dest != 6)
	{
		writeToRgister(dest, n);
	}
	MEMPTR = absoluteAddress;

	return 0;
}

uint8_t Z80::SRLIYD()
{
	int8_t d = readMemory(PC - 2);
	absoluteAddress = IY + d;

	uint8_t n = readMemory(absoluteAddress);

	bool newC = n & 1;
	n >>= 1;

	writeMemory(absoluteAddress, n);

	setFlag(Flags::C, newC);
	setRotateFlags(n, true);

	uint8_t dest = currentOpCode & 0b00000111;
	if (dest != 6)
	{
		writeToRgister(dest, n);
	}
	MEMPTR = absoluteAddress;

	return 0;
}

uint8_t Z80::RLD()
{
	absoluteAddress = readRegisterPair(RegisterPairs::HL);
	uint8_t n = readMemory(absoluteAddress);

	uint8_t lowNibbleA = A & 0xFF;
	uint8_t hiNibbleA = A & 0xFF00;
	uint8_t lowNibbleN = n & 0xFF;
	uint8_t hiNibbleN = n & 0xFF00;

	A = hiNibbleA | (hiNibbleN >> 4);
	n = (lowNibbleN << 4) | lowNibbleA;

	writeMemory(absoluteAddress, n);

	setFlag(Flags::S, A & 0x80);
	setFlag(Flags::Z, !A);
	setFlag(Flags::H, false);
	setFlag(Flags::P, parity(A));
	setFlag(Flags::N, false);

	setFlag(Flags::X, A & 0x08);
	setFlag(Flags::U, A & 0x20);
	setQ();

	MEMPTR = ++absoluteAddress;

	return 0;
}

uint8_t Z80::RRD()
{
	absoluteAddress = readRegisterPair(RegisterPairs::HL);
	uint8_t n = readMemory(absoluteAddress);

	uint8_t lowNibbleA = A & 0xFF;
	uint8_t hiNibbleA = A & 0xFF00;
	uint8_t lowNibbleN = n & 0xFF;
	uint8_t hiNibbleN = n & 0xFF00;

	A = hiNibbleA | lowNibbleN;
	n = (lowNibbleA << 4) | (hiNibbleN >> 4);

	writeMemory(absoluteAddress, n);

	setFlag(Flags::S, A & 0x80);
	setFlag(Flags::Z, !A);
	setFlag(Flags::H, false);
	setFlag(Flags::P, parity(A));
	setFlag(Flags::N, false);

	setFlag(Flags::X, A & 0x08);
	setFlag(Flags::U, A & 0x20);
	setQ();

	MEMPTR = ++absoluteAddress;

	return 0;
}

//Undocumented

uint8_t Z80::SLSR()
{
	uint8_t src = currentOpCode & 0b00000111;
	uint8_t n = readFromRegister(src);

	bool newC = n & 0x80;
	n = (n << 1) | 1;

	writeToRgister(src, n);

	setFlag(Flags::C, newC);
	setRotateFlags(n);

	return 0;
}

uint8_t Z80::SLSHL()
{
	uint8_t n = readMemory(readRegisterPair(RegisterPairs::HL));

	bool newC = n & 0x80;
	n = (n << 1) | 1;

	writeMemory(readRegisterPair(RegisterPairs::HL), n);

	setFlag(Flags::C, newC);
	setRotateFlags(n);

	return 0;
}

uint8_t Z80::SLSIXD()
{
	int8_t d = readMemory(PC - 2);
	absoluteAddress = IX + d;

	uint8_t n = readMemory(absoluteAddress);

	bool newC = n & 0x80;
	n = (n << 1) | 1;

	writeMemory(absoluteAddress, n);

	setFlag(Flags::C, newC);
	setRotateFlags(n);

	uint8_t dest = currentOpCode & 0b00000111;
	if (dest != 6)
	{
		writeToRgister(dest, n);
	}
	MEMPTR = absoluteAddress;

	return 0;
}

uint8_t Z80::SLSIYD()
{
	int8_t d = readMemory(PC - 2);
	absoluteAddress = IY + d;

	uint8_t n = readMemory(absoluteAddress);

	bool newC = n & 0x80;
	n = (n << 1) | 1;

	writeMemory(absoluteAddress, n);

	setFlag(Flags::C, newC);
	setRotateFlags(n);

	uint8_t dest = currentOpCode & 0b00000111;
	if (dest != 6)
	{
		writeToRgister(dest, n);
	}
	MEMPTR = absoluteAddress;

	return 0;
}
