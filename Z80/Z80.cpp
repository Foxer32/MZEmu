#include "Z80.h"
#include "IBus.h"

Z80::Z80()
{
	genINT = genNMI = false;
	interruptData = { 0xFF, 0xFF, 0xFF, 0xFF };
	reset(true);
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

uint8_t Z80::step()
{
	clockCycles = 0;
	refresh = 2;

	uint8_t op1 = 0;
	uint8_t op2 = 0;

	if (!isHalted)
	{
		currentOpCode = readMemoryNext();
		op1 = currentOpCode;
		switch (currentOpCode)
		{
		case 0xDD:
		case 0xFD:
			irIsIX = (currentOpCode == 0xDD);
			IR = irIsIX ? IX : IY;

			currentOpCode = readMemoryNext();

			if (currentOpCode == 0xCB)
			{
				PC++;
				currentOpCode = readMemoryNext();
				op2 = currentOpCode;
				currentInstruction = &ircbInstructions[currentOpCode];
				break;
			}

			currentInstruction = &irInstructions[currentOpCode];
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
			refresh = 1;
			currentInstruction = &rootInstructions[currentOpCode];
			break;
		}

		incrementRefreshRegister(refresh);

		if (currentInstruction->op == &Z80::XXX)
		{
			uint8_t op3 = currentOpCode;
			uint8_t a = 1;
		}

		clockCycles = (*currentInstruction).tCycles;
		clockCycles += (this->*((*currentInstruction).op))();
	}
	else
	{
		clockCycles += 4;
	}

	handleInterrupts();

	return clockCycles;
}

void Z80::incrementRefreshRegister(uint8_t steps)
{
	bool msbSet = R & 0x80;
	R = (R & 0x7F) + steps;

	if (msbSet)
		R |= 0x80;
	else
		R &= 0x7F;
}

void Z80::unhaltIfHalted()
{
	if (isHalted)
	{
		isHalted = false;
		PC++;
	}
}

void Z80::handleInterrupts()
{
	if (genINT)
	{
		modInstructionIfInterrupt();
		genINT = false;
		unhaltIfHalted();
		if (IFF1)
		{
			IFF1 = IFF2 = false;

			switch (interruptMode)
			{
			case 0:
				currentOpCode = interruptData[0];
				incrementRefreshRegister(1);
				currentInstruction = &rootInstructions[currentOpCode];
				clockCycles += (*currentInstruction).tCycles;
				clockCycles += (this->*((*currentInstruction).op))();
				resetQ();
				break;
			case 1:
				pushPC();
				PC = 0x0038;
				break;
			case 2:
				pushPC();
				PC = (I << 8) | interruptData[0];
				PC = readMemoryNext2Bytes();
				clockCycles += 6;
				break;
			}
			clockCycles += 13;
		}
	}

	if (genNMI)
	{
		modInstructionIfInterrupt();
		genNMI = false;
		unhaltIfHalted();
		pushPC();

		IFF2 = IFF1;
		IFF1 = false;
		PC = 0x0066;
		clockCycles += 17;
	}
}

void Z80::modInstructionIfInterrupt()
{
	uint8_t(Z80::*instruction)(void) = currentInstruction->op;

	if (instruction == &Z80::LDAI || instruction == &Z80::LDAR)
		setFlag(Flags::P, false);
}

void Z80::maskableInterrupt()
{
	genINT = true;
}

void Z80::nonMaskableInterrupt()
{
	genNMI = true;
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
	interruptMode = 0;
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
