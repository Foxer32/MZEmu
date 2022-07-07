#include "Z80.h"
#include "IBus.h"

Z80::Z80()
{

	A = F = B = C = D = E = H = L = 0x00;
	A1 = F1 = B1 = C1 = D1 = E1 = H1 = L1 = 0x00;
	I = R = 0x00;
	IX = IY = 0x0000;

	initInstructions();

}

Z80::~Z80()
{

}

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
	case 6:
		return F;
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
	case 6:
		F = v;
		break;
	case 7:
		A = v;
		break;
	}
}

void Z80::incRegisterPair(RegisterPairs rp, int16_t v)
{
	writeRegisterPair(rp, readRegisterPair(rp) + v);
}

void Z80::step()
{
	do
	{
		tick();
	} while (clockCycles != 0);
}

void Z80::tick()
{
	if (clockCycles == 0 && !isHalted)
	{	
		currentOpCode = readMemoryNext();
		
		switch (currentOpCode)
		{
		case 0xDD:
			currentOpCode = readMemoryNext();

			if (currentOpCode == 0xCB)
			{
				PC++;
				currentOpCode = readMemoryNext();
				currentInstruction = &ddcbInstructions[currentOpCode];
				break;
			}

			currentInstruction = &ddInstructions[currentOpCode];
			break;
		case 0xFD:
			currentOpCode = readMemoryNext();

			if (currentOpCode == 0xCB)
			{
				PC++;
				currentOpCode = readMemoryNext();
				currentInstruction = &fdcbInstructions[currentOpCode];
				break;
			}

			currentInstruction = &fdInstructions[currentOpCode];
			break;
		case 0xED:
			currentOpCode = readMemoryNext();

			currentInstruction = &edInstructions[currentOpCode];
			break;
		case 0xCB:
			currentOpCode = readMemoryNext();

			currentInstruction = &cbInstructions[currentOpCode];
			break;
		default:
			currentInstruction = &rootInstructions[currentOpCode];
			break;
		}

		clockCycles = (*currentInstruction).tCycles;
		clockCycles += (this->*((*currentInstruction).op))();
	}

	clockCycles--;
}

void Z80::reset(bool hardReset)
{
	clockCycles = 0;
	A = A1 = 0xFF;
	F = F1 = 0xFF;
	I = R = 0x00;
	PC = 0x0000;
	SP = 0xFFFF;
	IFF1 = IFF2 = false;
	interruptMode = InterruptModes::Mode0;
	isHalted = false;
	Q = 0x00;

	if (hardReset)
	{
		B = C = D = E = H = L = 0x00;
		B1 = C1 = D1 = E1 = H1 = L1 = 0x00;
		IX = IY = 0x0000;
		MEMPTR = 0x0000;
	}
}