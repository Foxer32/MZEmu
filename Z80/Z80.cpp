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

void Z80::setSampleFrequency(uint32_t sampleRate)
{
	cpuUpdFreq = (float)cpuFrequency / (float)sampleRate;
}

void Z80::updateCpu()
{
	float tCount = lastTCount;
	while (tCount < cpuUpdFreq)
		tCount += step();
	lastTCount = tCount - cpuUpdFreq;
}

uint8_t Z80::step()
{
	clockCycles = 0;
	refresh = 2;

	if (!isHalted)
	{
		currentOpCode = readMemoryNext();
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
