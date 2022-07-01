#include "Z80.h"

uint8_t Z80::ADDAR()
{

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

	return 0;
}

uint8_t Z80::ADDAIXD()
{

	return 0;
}

uint8_t Z80::ADDAIYD()
{

	return 0;
}

uint8_t Z80::ADCAR()
{

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

	return 0;
}

uint8_t Z80::ADCAIXD()
{

	return 0;
}

uint8_t Z80::ADCAIYD()
{

	return 0;
}

uint8_t Z80::SUBR()
{

	return 0;
}

uint8_t Z80::SUBN()
{

	return 0;
}

uint8_t Z80::SUBHL()
{

	return 0;
}

uint8_t Z80::SUBIXD()
{

	return 0;
}

uint8_t Z80::SUBIYD()
{

	return 0;
}

uint8_t Z80::SBCAR()
{

	return 0;
}

uint8_t Z80::SBCAN()
{

	return 0;
}

uint8_t Z80::SBCAHL()
{

	return 0;
}

uint8_t Z80::SBCAIXD()
{

	return 0;
}

uint8_t Z80::SBCAIYD()
{

	return 0;
}

uint8_t Z80::ANDR()
{

	return 0;
}

uint8_t Z80::ANDN()
{

	return 0;
}

uint8_t Z80::ANDHL()
{

	return 0;
}

uint8_t Z80::ANDIXD()
{

	return 0;
}

uint8_t Z80::ANDIYD()
{

	return 0;
}

uint8_t Z80::ORR()
{

	return 0;
}

uint8_t Z80::ORN()
{

	return 0;
}

uint8_t Z80::ORHL()
{

	return 0;
}

uint8_t Z80::ORIXD()
{

	return 0;
}

uint8_t Z80::ORIYD()
{

	return 0;
}

uint8_t Z80::XORR()
{

	return 0;
}

uint8_t Z80::XORN()
{

	return 0;
}

uint8_t Z80::XORHL()
{

	return 0;
}

uint8_t Z80::XORIXD()
{

	return 0;
}

uint8_t Z80::XORIYD()
{

	return 0;
}

uint8_t Z80::CPR()
{

	return 0;
}

uint8_t Z80::CPN()
{

	return 0;
}

uint8_t Z80::CPHL()
{

	return 0;
}

uint8_t Z80::CPIXD()
{

	return 0;
}

uint8_t Z80::CPIYD()
{

	return 0;
}

uint8_t Z80::INCR()
{

	return 0;
}

uint8_t Z80::INCHL()
{

	return 0;
}

uint8_t Z80::INCIXD()
{

	return 0;
}

uint8_t Z80::INCIYD()
{

	return 0;
}

uint8_t Z80::DECR()
{

	return 0;
}

uint8_t Z80::DECHL()
{

	return 0;
}

uint8_t Z80::DECIXD()
{

	return 0;
}

uint8_t Z80::DECIYD()
{

	return 0;
}
