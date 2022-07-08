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
	A = andAB(A, n);

	return 0;
}

uint8_t Z80::ANDN()
{
	uint8_t n = readMemoryNext();
	A = andAB(A, n);

	return 0;
}

uint8_t Z80::ANDHL()
{
	uint8_t n = readMemory(readRegisterPair(RegisterPairs::HL));
	A = andAB(A, n);

	return 0;
}

uint8_t Z80::ANDIRD()
{
	int8_t d = readMemoryNext();
	absoluteAddress = IR + d;
	uint8_t n = readMemory(absoluteAddress);
	A = andAB(A, n);

	MEMPTR = absoluteAddress;

	return 0;
}

uint8_t Z80::ORR()
{
	uint8_t n = readFromRegister(currentOpCode & 0b00000111);
	A = orAB(A, n);

	return 0;
}

uint8_t Z80::ORN()
{
	uint8_t n = readMemoryNext();
	A = orAB(A, n);

	return 0;
}

uint8_t Z80::ORHL()
{
	uint8_t n = readMemory(readRegisterPair(RegisterPairs::HL));
	A = orAB(A, n);

	return 0;
}

uint8_t Z80::ORIRD()
{
	int8_t d = readMemoryNext();
	absoluteAddress = IR + d;
	uint8_t n = readMemory(absoluteAddress);
	A = orAB(A, n);

	MEMPTR = absoluteAddress;

	return 0;
}

uint8_t Z80::XORR()
{
	uint8_t n = readFromRegister(currentOpCode & 0b00000111);
	A = xorAB(A, n);

	return 0;
}

uint8_t Z80::XORN()
{
	uint8_t n = readMemoryNext();
	A = xorAB(A, n);

	return 0;
}

uint8_t Z80::XORHL()
{
	uint8_t n = readMemory(readRegisterPair(RegisterPairs::HL));
	A = xorAB(A, n);

	return 0;
}

uint8_t Z80::XORIRD()
{
	int8_t d = readMemoryNext();
	absoluteAddress = IR + d;
	uint8_t n = readMemory(absoluteAddress);
	A = xorAB(A, n);

	MEMPTR = absoluteAddress;

	return 0;
}

uint8_t Z80::CPR()
{
	uint8_t n = readFromRegister(currentOpCode & 0b00000111);
	setComparsionFlags(n, A - n);

	return 0;
}

uint8_t Z80::CPN()
{
	uint8_t n = readMemoryNext();
	setComparsionFlags(n, A - n);

	return 0;
}

uint8_t Z80::CPHL()
{
	uint8_t n = readMemory(readRegisterPair(RegisterPairs::HL));
	setComparsionFlags(n, A - n);

	return 0;
}

uint8_t Z80::CPIRD()
{
	int8_t d = readMemoryNext();
	absoluteAddress = IR + d;
	uint8_t n = readMemory(absoluteAddress);
	setComparsionFlags(n, A - n);

	MEMPTR = absoluteAddress;

	return 0;
}

uint8_t Z80::INCR()
{
	uint8_t reg = (currentOpCode & 0b00111000) >> 3;

	uint8_t val = readFromRegister(reg);
	uint8_t incVal = val + 1;
	writeToRgister(reg, incVal);

	setIncFlags(val, incVal);

	return 0;
}

uint8_t Z80::INCHL()
{
	uint8_t val = readMemory(readRegisterPair(RegisterPairs::HL));

	uint8_t incVal = val + 1;
	writeMemory(readRegisterPair(RegisterPairs::HL), incVal);

	setIncFlags(val, incVal);

	return 0;
}

uint8_t Z80::INCIRD()
{
	int8_t d = readMemoryNext();
	absoluteAddress = IR + d;

	uint8_t val = readMemory(absoluteAddress);

	uint8_t incVal = val + 1;
	writeMemory(absoluteAddress, incVal);

	setIncFlags(val, incVal);
	MEMPTR = absoluteAddress;

	return 0;
}

uint8_t Z80::DECR()
{
	uint8_t reg = (currentOpCode & 0b00111000) >> 3;

	uint8_t val = readFromRegister(reg);
	uint8_t decVal = val - 1;
	writeToRgister(reg, decVal);

	setDecFlags(val, decVal);

	return 0;
}

uint8_t Z80::DECHL()
{
	uint8_t val = readMemory(readRegisterPair(RegisterPairs::HL));

	uint8_t decVal = val - 1;
	writeMemory(readRegisterPair(RegisterPairs::HL), decVal);

	setDecFlags(val, decVal);

	return 0;
}

uint8_t Z80::DECIRD()
{
	int8_t d = readMemoryNext();
	absoluteAddress = IR + d;

	uint8_t val = readMemory(absoluteAddress);

	uint8_t decVal = val - 1;
	writeMemory(absoluteAddress, decVal);

	setDecFlags(val, decVal);
	MEMPTR = absoluteAddress;

	return 0;
}
