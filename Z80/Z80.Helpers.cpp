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




