#include "Z80.h"

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
	default:
		break;
	}
}

uint8_t Z80::add8(uint8_t a, uint8_t b, uint8_t c)
{
	uint16_t sum = a + b + c;

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
	setFlag(Flags::P, (~(a ^ b) & (b ^ diff)) & 0x80);
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
		{
			retVal = !retVal;
		}

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
	setFlag(Flags::H, true);
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
	setFlag(Flags::H, true);
	setFlag(Flags::P, parity(res));
	setFlag(Flags::N, false);
	setFlag(Flags::C, false);

	setFlag(Flags::X, res & 0x08);
	setFlag(Flags::U, res & 0x20);
	setQ();

	return res;
}

void Z80::setComparsionFlags(uint8_t n, uint16_t diff)
{
	setFlag(Flags::S, diff & 0x80);
	setFlag(Flags::Z, !diff);
	setFlag(Flags::H, (A & 0x0F) < (n & 0x0F));
	setFlag(Flags::P, (~(A ^ n) & (n ^ diff)) & 0x80);
	setFlag(Flags::N, true);
	setFlag(Flags::C, n > A);

	setFlag(Flags::X, diff & 0x08);
	setFlag(Flags::U, diff & 0x20);
	setQ();
}

void Z80::setIncFlags(uint8_t val, uint8_t incVal)
{
	setFlag(Flags::S, incVal & 0x80);
	setFlag(Flags::Z, !incVal);
	setFlag(Flags::H, ((val & 0x0F) + (val & 0x0F)) & 0xF0);
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
	setFlag(Flags::H, ((val & 0x0F) + (val & 0x0F)) & 0xF0);
	setFlag(Flags::P, val == 0x80);
	setFlag(Flags::N, true);

	setFlag(Flags::X, decVal & 0x08);
	setFlag(Flags::U, decVal & 0x20);
	setQ();
}

