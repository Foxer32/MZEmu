#include "Z80.h"

uint8_t Z80::ADDAR()
{
	uint8_t n = readFromRegister(currentOpCode & 0b00000111);
	A = add8(A, n);

	return 0;
}

uint8_t Z80::ADDAN()
{
	uint8_t n = readMemoryNext();
	A = add8(A, n);

	return 0;
}

uint8_t Z80::ADDAHL()
{
	uint8_t n = readMemory(readRegisterPair(RegisterPairs::HL));
	A = add8(A, n);

	return 0;
}

uint8_t Z80::ADDAIRD()
{
	int8_t d = readMemoryNext();
	absoluteAddress = IR + d;
	uint8_t n = readMemory(absoluteAddress);
	A = add8(A, n);

	MEMPTR = absoluteAddress;

	return 0;
}

uint8_t Z80::ADCAR()
{
	uint8_t n = readFromRegister(currentOpCode & 0b00000111);
	A = add8(A, n, getFlag(Flags::C));

	return 0;
}

uint8_t Z80::ADCAN()
{
	uint8_t n = readMemoryNext();
	A = add8(A, n, getFlag(Flags::C));

	return 0;
}

uint8_t Z80::ADCAHL()
{
	uint8_t n = readMemory(readRegisterPair(RegisterPairs::HL));
	A = add8(A, n, getFlag(Flags::C));

	return 0;
}

uint8_t Z80::ADCAIRD()
{
	int8_t d = readMemoryNext();
	absoluteAddress = IR + d;
	uint8_t n = readMemory(absoluteAddress);
	A = add8(A, n, getFlag(Flags::C));

	MEMPTR = absoluteAddress;

	return 0;
}

uint8_t Z80::SUBR()
{
	uint8_t n = readFromRegister(currentOpCode & 0b00000111);
	A = sub8(A, n);

	return 0;
}

uint8_t Z80::SUBN()
{
	uint8_t n = readMemoryNext();
	A = sub8(A, n);

	return 0;
}

uint8_t Z80::SUBHL()
{
	uint8_t n = readMemory(readRegisterPair(RegisterPairs::HL));
	A = sub8(A, n);

	return 0;
}

uint8_t Z80::SUBIRD()
{
	int8_t d = readMemoryNext();
	absoluteAddress = IR + d;
	uint8_t n = readMemory(absoluteAddress);
	A = sub8(A, n);

	MEMPTR = absoluteAddress;

	return 0;
}

uint8_t Z80::SBCAR()
{
	uint8_t n = readFromRegister(currentOpCode & 0b00000111);
	A = sub8(A, n, getFlag(Flags::C));

	return 0;
}

uint8_t Z80::SBCAN()
{
	uint8_t n = readMemoryNext();
	A = sub8(A, n, getFlag(Flags::C));

	return 0;
}

uint8_t Z80::SBCAHL()
{
	uint8_t n = readMemory(readRegisterPair(RegisterPairs::HL));
	A = sub8(A, n, getFlag(Flags::C));

	return 0;
}

uint8_t Z80::SBCAIRD()
{
	int8_t d = readMemoryNext();
	absoluteAddress = IR + d;
	uint8_t n = readMemory(absoluteAddress);
	A = sub8(A, n, getFlag(Flags::C));

	MEMPTR = absoluteAddress;

	return 0;
}

uint8_t Z80::ANDR()
{
	uint8_t n = readFromRegister(currentOpCode & 0b00000111);
	andAN(n);

	return 0;
}

uint8_t Z80::ANDN()
{
	uint8_t n = readMemoryNext();
	andAN(n);

	return 0;
}

uint8_t Z80::ANDHL()
{
	uint8_t n = readMemory(readRegisterPair(RegisterPairs::HL));
	andAN(n);

	return 0;
}

uint8_t Z80::ANDIRD()
{
	int8_t d = readMemoryNext();
	absoluteAddress = IR + d;
	uint8_t n = readMemory(absoluteAddress);
	andAN(n);

	MEMPTR = absoluteAddress;

	return 0;
}

uint8_t Z80::ORR()
{
	uint8_t n = readFromRegister(currentOpCode & 0b00000111);
	orAN(n);

	return 0;
}

uint8_t Z80::ORN()
{
	uint8_t n = readMemoryNext();
	orAN(n);

	return 0;
}

uint8_t Z80::ORHL()
{
	uint8_t n = readMemory(readRegisterPair(RegisterPairs::HL));
	orAN(n);

	return 0;
}

uint8_t Z80::ORIRD()
{
	int8_t d = readMemoryNext();
	absoluteAddress = IR + d;
	uint8_t n = readMemory(absoluteAddress);
	orAN(n);

	MEMPTR = absoluteAddress;

	return 0;
}

uint8_t Z80::XORR()
{
	uint8_t n = readFromRegister(currentOpCode & 0b00000111);
	xorAN(n);

	return 0;
}

uint8_t Z80::XORN()
{
	uint8_t n = readMemoryNext();
	xorAN(n);

	return 0;
}

uint8_t Z80::XORHL()
{
	uint8_t n = readMemory(readRegisterPair(RegisterPairs::HL));
	xorAN(n);

	return 0;
}

uint8_t Z80::XORIRD()
{
	int8_t d = readMemoryNext();
	absoluteAddress = IR + d;
	uint8_t n = readMemory(absoluteAddress);
	xorAN(n);

	MEMPTR = absoluteAddress;

	return 0;
}

uint8_t Z80::CPR()
{
	uint8_t n = readFromRegister(currentOpCode & 0b00000111);
	compareAN(n);

	return 0;
}

uint8_t Z80::CPN()
{
	uint8_t n = readMemoryNext();
	compareAN(n);

	return 0;
}

uint8_t Z80::CPHL()
{
	uint8_t n = readMemory(readRegisterPair(RegisterPairs::HL));
	compareAN(n);

	return 0;
}

uint8_t Z80::CPIRD()
{
	int8_t d = readMemoryNext();
	absoluteAddress = IR + d;
	uint8_t n = readMemory(absoluteAddress);
	compareAN(n);

	MEMPTR = absoluteAddress;

	return 0;
}

uint8_t Z80::INCR()
{
	uint8_t reg = (currentOpCode & 0b00111000) >> 3;
	uint8_t val = readFromRegister(reg);
	writeToRgister(reg, incN(val));

	return 0;
}

uint8_t Z80::INCHL()
{
	uint8_t val = readMemory(readRegisterPair(RegisterPairs::HL));
	writeMemory(readRegisterPair(RegisterPairs::HL), incN(val));

	return 0;
}

uint8_t Z80::INCIRD()
{
	int8_t d = readMemoryNext();
	absoluteAddress = IR + d;

	uint8_t val = readMemory(absoluteAddress);

	writeMemory(absoluteAddress, incN(val));

	MEMPTR = absoluteAddress;

	return 0;
}

uint8_t Z80::DECR()
{
	uint8_t reg = (currentOpCode & 0b00111000) >> 3;
	uint8_t val = readFromRegister(reg);
	writeToRgister(reg, decN(val));

	return 0;
}

uint8_t Z80::DECHL()
{
	uint8_t val = readMemory(readRegisterPair(RegisterPairs::HL));
	writeMemory(readRegisterPair(RegisterPairs::HL), decN(val));

	return 0;
}

uint8_t Z80::DECIRD()
{
	int8_t d = readMemoryNext();
	absoluteAddress = IR + d;

	uint8_t val = readMemory(absoluteAddress);

	writeMemory(absoluteAddress, decN(val));

	MEMPTR = absoluteAddress;

	return 0;
}
