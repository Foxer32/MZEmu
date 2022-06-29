#include "Z80.h"

uint8_t Z80::ADDAN()
{
	uint8_t n = readMemoryNext();
	A = add8(A, n);

	return 0;
}

uint8_t Z80::ADCAN()
{
	uint8_t n = readMemoryNext();
	A = add8(A, n, getFlag(Flags::C));

	return 0;
}


