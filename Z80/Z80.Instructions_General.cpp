#include "Z80.h"

uint8_t Z80::XXX()
{

	return 0;
}

uint8_t Z80::DAA()
{
    uint8_t t = 0;

    if (getFlag(Flags::H) || ((A & 0x0F) > 9))
    {
        t++;
    }

    if (getFlag(Flags::C) || (A > 0x99))
    {
        t += 2;
        setFlag(Flags::C, true);
    }

    if (getFlag(Flags::N) && !getFlag(Flags::H))
    {
        setFlag(Flags::H, false);
    }
    else
    {
        if (getFlag(Flags::N) && getFlag(Flags::H))
        {
            setFlag(Flags::H, (A & 0x0F) < 6);
        }
        else
        {
            setFlag(Flags::H, (A & 0x0F) >= 0x0A);
        }
    }

    switch (t)
    {
    case 1:
        A += getFlag(Flags::N) ? 0xFA : 0x06;
        break;
    case 2:
        A += getFlag(Flags::N) ? 0xA0 : 0x60;
        break;
    case 3:
        A += getFlag(Flags::N) ? 0x9A : 0x66;
        break;
    }

	setFlag(Flags::S, A & 0x80);
	setFlag(Flags::Z, !A);
	setFlag(Flags::P, parity(A));

	setFlag(Flags::X, A & 0x08);
	setFlag(Flags::U, A & 0x20);
	setQ();

	return 0;
}

uint8_t Z80::CPL()
{
    A = ~A;

    setFlag(Flags::H, true);
    setFlag(Flags::N, true);

    setFlag(Flags::X, A & 0x08);
    setFlag(Flags::U, A & 0x20);
    setQ();

	return 0;
}

uint8_t Z80::NEG()
{
    uint8_t temp = A;
    A = ~A + 1;

    setFlag(Flags::S, A & 0x80);
    setFlag(Flags::Z, !A);
    setFlag(Flags::H, ((temp & 0x0F) + (A & 0x0F)) & 0xF0);
    setFlag(Flags::P, A == 0x80);
    setFlag(Flags::N, true);
    setFlag(Flags::C, temp);

    setFlag(Flags::X, A & 0x08);
    setFlag(Flags::U, A & 0x20);
    setQ();

	return 0;
}

uint8_t Z80::CCF()
{
    bool temp = getFlag(Flags::C);
    setFlag(Flags::C, !temp);
    setFlag(Flags::H, temp);
    setFlag(Flags::N, false);

    uint8_t x = (Q ^ F) | A;
    setFlag(Flags::X, x & 0x08);
    setFlag(Flags::U, x & 0x20);
    setQ();

	return 0;
}

uint8_t Z80::SCF()
{
    setFlag(Flags::C, true);
    setFlag(Flags::H, false);
    setFlag(Flags::N, false);

    uint8_t x = (Q ^ F) | A;
    setFlag(Flags::X, x & 0x08);
    setFlag(Flags::U, x & 0x20);
    setQ();

	return 0;
}

uint8_t Z80::NOP()
{

	return 0;
}

uint8_t Z80::HALT()
{
    if (!isHalted)
    {
        isHalted = true;
        PC--;
    }

    resetQ();
	return 0;
}

uint8_t Z80::DI()
{
    IFF1 = IFF2 = false;
    resetQ();

	return 0;
}

uint8_t Z80::EI()
{
    IFF1 = IFF2 = true;
    resetQ();

	return 0;
}

uint8_t Z80::IM0()
{
    interruptMode = InterruptModes::Mode0;
    resetQ();

	return 0;
}

uint8_t Z80::IM1()
{
    interruptMode = InterruptModes::Mode1;
    resetQ();

	return 0;
}

uint8_t Z80::IM2()
{
    interruptMode = InterruptModes::Mode2;
    resetQ();

	return 0;
}
