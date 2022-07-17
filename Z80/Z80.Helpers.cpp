#include "Z80.h"
#include "IBus.h"

void Z80::setQ()
{
	Q = F;
}

void Z80::resetQ()
{
	Q = 0x00;
}

bool Z80::getFlag(Flags flag)
{
	return F & static_cast<uint8_t>(flag);
}

void Z80::setFlag(Flags flag, bool v)
{
	if (v)
		F |= static_cast<uint8_t>(flag);
	else
		F &= ~static_cast<uint8_t>(flag);
}

uint8_t Z80::readMemoryNext()
{
	return bus->readMemory(PC++);
}

uint16_t Z80::readMemoryNext2Bytes()
{
	uint8_t loByte = readMemoryNext();
	uint8_t hiByte = readMemoryNext();

	return (hiByte << 8) | loByte;
}

uint8_t Z80::readMemory(uint16_t addr)
{
	return bus->readMemory(addr);
}

void Z80::writeMemory(uint16_t addr, uint8_t data)
{
	bus->writeMemory(addr, data);
}

uint8_t Z80::readPeripheral(uint16_t addr)
{
	return bus->readPeripheral(addr);
}

void Z80::writePeripheral(uint16_t addr, uint8_t data)
{
	bus->writePeripheral(addr, data);
}

void Z80::incRegisterPair(RegisterPairs rp, int16_t v)
{
	writeRegisterPair(rp, readRegisterPair(rp) + v);
}

void Z80::writeRegisterPair(RegisterPairs dest, uint16_t v)
{
	switch (static_cast<uint8_t>(dest))
	{
	case 0:
		A = v >> 8;
		F = v & 0xFF;
		break;
	case 1:
		H = v >> 8;
		L = v & 0xFF;
		break;
	case 2:
		B = v >> 8;
		C = v & 0xFF;
		break;
	case 3:
		D = v >> 8;
		E = v & 0xFF;
		break;
	case 4:
		A1 = v >> 8;
		F1 = v & 0xFF;
		break;
	case 5:
		H1 = v >> 8;
		L1 = v & 0xFF;
		break;
	case 6:
		B1 = v >> 8;
		C1 = v & 0xFF;
		break;
	case 7:
		D1 = v >> 8;
		E1 = v & 0xFF;
		break;
	}
}

uint16_t Z80::readRegisterPair(RegisterPairs src)
{
	switch (static_cast<uint8_t>(src))
	{
	case 0:
		return (A << 8) | F;
	case 1:
		return (H << 8) | L;
	case 2:
		return (B << 8) | C;
	case 3:
		return (D << 8) | E;
	case 4:
		return (A1 << 8) | F1;
	case 5:
		return (H1 << 8) | L1;
	case 6:
		return (B1 << 8) | C1;
	case 7:
		return (D1 << 8) | E1;
	default:
		return 0x0000;
	}
}

uint8_t Z80::readFromRegister(uint8_t src)
{
	switch (src)
	{
	case 0:
		return B;
	case 1:
		return C;
	case 2:
		return D;
	case 3:
		return E;
	case 4:
		return H;
	case 5:
		return L;
		//case 6:
		//	return F;
	case 7:
		return A;
	default:
		return 0x00;
	}
}

void Z80::writeToRgister(uint8_t dest, uint8_t v)
{
	switch (dest)
	{
	case 0:
		B = v;
		break;
	case 1:
		C = v;
		break;
	case 2:
		D = v;
		break;
	case 3:
		E = v;
		break;
	case 4:
		H = v;
		break;
	case 5:
		L = v;
		break;
		//case 6:
		//	F = v;
		//	break;
	case 7:
		A = v;
		break;
	}
}

//---------------------------------------------------------------

void Z80::writeDD(uint16_t v, uint8_t dd)
{
	switch (dd)
	{
	case 0:
		writeRegisterPair(RegisterPairs::BC, v);
		break;
	case 1:
		writeRegisterPair(RegisterPairs::DE, v);
		break;
	case 2:
		writeRegisterPair(RegisterPairs::HL, v);
		break;
	case 3:
		SP = v;
		break;
	}
}

uint8_t Z80::add8(uint8_t a, uint8_t b, uint8_t c)
{
	uint16_t sum = a + b + c;
	b += c;

	setFlag(Flags::S, sum & 0x80);
	setFlag(Flags::Z, !(sum & 0xFF));
	setFlag(Flags::H, ((a & 0x0F) + (b & 0x0F)) & 0xF0);
	setFlag(Flags::P, (~(a ^ b) & (a ^ sum)) & 0x80);
	setFlag(Flags::N, false);
	setFlag(Flags::C, sum & 0xFF00);

	setFlag(Flags::X, sum & 0x08);
	setFlag(Flags::U, sum & 0x20);
	setQ();

	return sum;
}

uint8_t Z80::sub8(uint8_t a, uint8_t b, uint8_t c)
{
	uint16_t diff = a - b - c;
	b += c;

	setFlag(Flags::S, diff & 0x80);
	setFlag(Flags::Z, !(diff & 0xFF));
	setFlag(Flags::H, (a & 0x0F) < (b & 0x0F));
	setFlag(Flags::P, ((a ^ b) & ~(b ^ diff)) & 0x80);
	setFlag(Flags::N, true);
	setFlag(Flags::C, diff & 0xFF00);

	setFlag(Flags::X, diff & 0x08);
	setFlag(Flags::U, diff & 0x20);
	setQ();

	return diff;
}

bool Z80::parity(uint16_t v)
{
	bool retVal = true;

	while (v > 0)
	{
		if (v & 0x01)
			retVal = !retVal;

		v >>= 1;
	}

	return retVal;
}

uint8_t Z80::andAB(uint8_t a, uint8_t b)
{
	uint8_t res = a & b;

	setFlag(Flags::S, res & 0x80);
	setFlag(Flags::Z, !res);
	setFlag(Flags::H, true);
	setFlag(Flags::P, parity(res));
	setFlag(Flags::N, false);
	setFlag(Flags::C, false);

	setFlag(Flags::X, res & 0x08);
	setFlag(Flags::U, res & 0x20);
	setQ();

	return res;
}

uint8_t Z80::orAB(uint8_t a, uint8_t b)
{
	uint8_t res = a | b;

	setFlag(Flags::S, res & 0x80);
	setFlag(Flags::Z, !res);
	setFlag(Flags::H, false);
	setFlag(Flags::P, parity(res));
	setFlag(Flags::N, false);
	setFlag(Flags::C, false);

	setFlag(Flags::X, res & 0x08);
	setFlag(Flags::U, res & 0x20);
	setQ();

	return res;
}

uint8_t Z80::xorAB(uint8_t a, uint8_t b)
{
	uint8_t res = a ^ b;

	setFlag(Flags::S, res & 0x80);
	setFlag(Flags::Z, !res);
	setFlag(Flags::H, false);
	setFlag(Flags::P, parity(res));
	setFlag(Flags::N, false);
	setFlag(Flags::C, false);

	setFlag(Flags::X, res & 0x08);
	setFlag(Flags::U, res & 0x20);
	setQ();

	return res;
}

void Z80::setComparsionFlags(uint8_t n, uint8_t diff)
{
	setFlag(Flags::S, diff & 0x80);
	setFlag(Flags::Z, !diff);
	setFlag(Flags::H, (A & 0x0F) < (n & 0x0F));
	setFlag(Flags::P, ((A ^ n) & ~(n ^ diff)) & 0x80);
	setFlag(Flags::N, true);
	setFlag(Flags::C, n > A);

	setFlag(Flags::X, n & 0x08);
	setFlag(Flags::U, n & 0x20);
	setQ();
}

void Z80::setIncFlags(uint8_t val, uint8_t incVal)
{
	setFlag(Flags::S, incVal & 0x80);
	setFlag(Flags::Z, !incVal);
	setFlag(Flags::H, ((val & 0x0F) + 1) & 0xF0);
	setFlag(Flags::P, val == 0x7F);
	setFlag(Flags::N, false);

	setFlag(Flags::X, incVal & 0x08);
	setFlag(Flags::U, incVal & 0x20);
	setQ();
}

void Z80::setDecFlags(uint8_t val, uint8_t decVal)
{
	setFlag(Flags::S, decVal & 0x80);
	setFlag(Flags::Z, !decVal);
	setFlag(Flags::H, !(val & 0x0F));
	setFlag(Flags::P, val == 0x80);
	setFlag(Flags::N, true);

	setFlag(Flags::X, decVal & 0x08);
	setFlag(Flags::U, decVal & 0x20);
	setQ();
}

uint16_t Z80::readRegisterPair2(uint8_t src, uint16_t self, bool haveSelf)
{
	switch (src)
	{
	case 0:
		return readRegisterPair(RegisterPairs::BC);
	case 1:
		return readRegisterPair(RegisterPairs::DE);
	case 2:
		return haveSelf? self : readRegisterPair(RegisterPairs::HL);
	case 3:
		return SP;
	default:
		return 0x0000;
	}
}

void Z80::writeRegisterPair2(uint8_t dest, uint16_t v)
{
	switch (dest)
	{
	case 0:
		B = v >> 8;
		C = v & 0xFF;
		break;
	case 1:
		D = v >> 8;
		E = v & 0xFF;
		break;
	case 2:
		H = v >> 8;
		L = v & 0xFF;
		break;
	case 3:
		SP = v;
		break;
	}
}

uint16_t Z80::add16(uint16_t a, uint16_t b, uint8_t c, bool withCarry)
{
	uint32_t sum = a + b + c;
	b += c;

	setFlag(Flags::H, ((a & 0x0FFF) + (b & 0x0FFF)) & 0xF000);
	setFlag(Flags::N, false);
	setFlag(Flags::C, sum & 0xFFFF0000);

	if (withCarry)
	{
		setFlag(Flags::S, sum & 0x8000);
		setFlag(Flags::Z, !(sum & 0xFFFF));
		setFlag(Flags::P, (~(a ^ b) & (a ^ sum)) & 0x8000);
	}

	setFlag(Flags::X, sum & 0x0800);
	setFlag(Flags::U, sum & 0x2000);

	MEMPTR = ++a;

	setQ();

	return sum;
}

uint16_t Z80::sub16(uint16_t a, uint16_t b, uint8_t c)
{
	uint32_t diff = a - b - c;
	b += c;

	setFlag(Flags::S, diff & 0x8000);
	setFlag(Flags::Z, !(diff & 0xFFFF));
	setFlag(Flags::H, (a & 0x0FFF) < (b & 0x0FFF));
	setFlag(Flags::P, ((a ^ b) & ~(b ^ diff)) & 0x8000);
	setFlag(Flags::N, true);
	setFlag(Flags::C, diff & 0xFFFF0000);

	setFlag(Flags::X, diff & 0x0800);
	setFlag(Flags::U, diff & 0x2000);

	MEMPTR = ++a;

	setQ();

	return diff;
}

void Z80::setRotateFlags(uint8_t n, bool resetS)
{
	setFlag(Flags::S, (n & 0x80) && !resetS);
	setFlag(Flags::Z, !n);
	setFlag(Flags::H, false);
	setFlag(Flags::P, parity(n));
	setFlag(Flags::N, false);

	setFlag(Flags::X, n & 0x08);
	setFlag(Flags::U, n & 0x20);
	setQ();
}

bool Z80::evaluateCC(uint8_t cc)
{
	switch (cc)
	{
	case 0:
		return !getFlag(Flags::Z);
	case 1:
		return getFlag(Flags::Z);
	case 2:
		return !getFlag(Flags::C);
	case 3:
		return getFlag(Flags::C);
	case 4:
		return !getFlag(Flags::P);
	case 5:
		return getFlag(Flags::P);
	case 6:
		return !getFlag(Flags::S);
	case 7:
		return getFlag(Flags::S);
	default:
		return false;
	}
}

void Z80::pushPC()
{
	uint8_t loByte = PC & 0xFF;
	uint8_t hiByte = PC >> 8;

	writeMemory(--SP, hiByte);
	writeMemory(--SP, loByte);
	resetQ();
}

void Z80::popPC()
{
	uint8_t loByte = readMemory(SP++);
	uint8_t hiByte = readMemory(SP++);

	PC = (hiByte << 8) | loByte;
	resetQ();
}

void Z80::setUndocIOFlags(uint16_t k, uint8_t n)
{
	uint16_t x = (k & 7) ^ B;
	setFlag(Flags::H, k & 0xFF00);
	setFlag(Flags::P, parity(x));
	setFlag(Flags::N, n & 0x80);
	setFlag(Flags::C, k & 0xFF00);
	setQ();
}

void Z80::saveIR()
{
	if (irIsIX)
		IX = IR;
	else
		IY = IR;
}

//---------------------------------------------------------------

uint8_t Z80::LDCPr(bool c)
{
	if (c)
	{
		PC -= 2;
		MEMPTR = PC + 1;
		return 5;
	}

	return 0;
}

uint8_t Z80::JRif(bool c)
{
	resetQ();

	if (c)
	{
		PC += (int8_t)readMemoryNext();
		MEMPTR = PC;
		return 5;
	}

	PC++;
	return 0;
}

uint8_t Z80::IOr()
{
	if (B)
	{
		PC -= 2;
		return 5;
	}

	return 0;
}
