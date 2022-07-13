#include "Z80.h"

uint8_t Z80::INAN()
{
	uint8_t port = readMemoryNext();

	absoluteAddress = (A << 8) | port;

	A = readPeripheral(absoluteAddress);

	resetQ();
	MEMPTR = ++absoluteAddress;

	return 0;
}

uint8_t Z80::INRC()
{
	absoluteAddress = readRegisterPair(RegisterPairs::BC);
	uint8_t n = readPeripheral(absoluteAddress);
	writeToRgister((currentOpCode & 0b00111000) >> 3, n);

	setFlag(Flags::S, n & 0x80);
	setFlag(Flags::Z, !n);
	setFlag(Flags::H, false);
	setFlag(Flags::P, parity(n));
	setFlag(Flags::N, false);

	MEMPTR = ++absoluteAddress;
	setFlag(Flags::X, n & 0x08);
	setFlag(Flags::U, n & 0x20);
	setQ();

	return 0;
}

uint8_t Z80::INI()
{
	absoluteAddress = readRegisterPair(RegisterPairs::BC);
	uint8_t n = readPeripheral(absoluteAddress);
	writeMemory(readRegisterPair(RegisterPairs::HL), n);

	incRegisterPair(RegisterPairs::HL);
	uint8_t val = B;
	B -= 1;
	setDecFlags(val,B);

	MEMPTR = ++absoluteAddress;
	setUndocIOFlags(n + ((C + 1) & 0xFF), n);

	return 0;
}

uint8_t Z80::INIR()
{
	INI();

	return IOr();
}

uint8_t Z80::IND()
{
	absoluteAddress = readRegisterPair(RegisterPairs::BC);
	uint8_t n = readPeripheral(absoluteAddress);
	writeMemory(readRegisterPair(RegisterPairs::HL), n);

	incRegisterPair(RegisterPairs::HL, -1);
	uint8_t val = B;
	B -= 1;
	setDecFlags(val, B);

	MEMPTR = --absoluteAddress;
	setUndocIOFlags(n + ((C - 1) & 0xFF), n);

	return 0;
}

uint8_t Z80::INDR()
{
	IND();

	return IOr();
}

uint8_t Z80::OUTNA()
{
	uint8_t port = readMemoryNext();

	writePeripheral((A << 8) | port, A);

	resetQ();
	MEMPTR = (A << 8) | (++port & 0xFF);

	return 0;
}

uint8_t Z80::OUTCR()
{
	absoluteAddress = readRegisterPair(RegisterPairs::BC);
	uint8_t n = readFromRegister((currentOpCode & 0b00111000) >> 3);
	writePeripheral(absoluteAddress, n);

	MEMPTR = ++absoluteAddress;
	resetQ();

	return 0;
}

uint8_t Z80::OUTI()
{
	absoluteAddress = readRegisterPair(RegisterPairs::BC);
	uint8_t n = readMemory(readRegisterPair(RegisterPairs::HL));
	writePeripheral(absoluteAddress, n);

	incRegisterPair(RegisterPairs::HL);
	uint8_t val = B;
	B -= 1;
	setDecFlags(val, B);

	MEMPTR = readRegisterPair(RegisterPairs::BC) + 1;
	setUndocIOFlags(n + L, n);

	return 0;
}

uint8_t Z80::OTIR()
{
	OUTI();

	return IOr();
}

uint8_t Z80::OUTD()
{
	absoluteAddress = readRegisterPair(RegisterPairs::BC);
	uint8_t n = readMemory(readRegisterPair(RegisterPairs::HL));
	writePeripheral(absoluteAddress, n);

	incRegisterPair(RegisterPairs::HL, -1);
	uint8_t val = B;
	B -= 1;
	setDecFlags(val, B);

	MEMPTR = readRegisterPair(RegisterPairs::BC) - 1;
	setUndocIOFlags(n + L, n);

	return 0;
}

uint8_t Z80::OTDR()
{
	OUTD();

	return IOr();
}
