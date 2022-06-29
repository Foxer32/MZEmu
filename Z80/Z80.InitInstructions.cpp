#include "Z80.h"

void Z80::initInstructions()
{
	using c = Z80;
	using a = AddressingModes;

	rootInstructions =
	{
		{ 0x00, {"NOP", a::IDX, &c::NOP, 4} },

		{ 0x02, {"LD (BC),A", a::IMP, &c::LDBCA, 7} },

		{ 0x06, {"LD B,n", a::IMM, &c::LDRN, 7} },

		{ 0x0A, {"LD A,(BC)", a::IDX, &c::LDABC, 7} },

		{ 0x0E, {"LD C,n", a::IMM, &c::LDRN, 7} },

		{ 0x12, {"LD (DE),A", a::IMP, &c::LDDEA, 7} },

		{ 0x16, {"LD D,n", a::IMM, &c::LDRN, 7} },

		{ 0x1A, {"LD A,(DE)", a::IDX, &c::LDADE, 7} },

		{ 0x1E, {"LD E,n", a::IMM, &c::LDRN, 7} },
		{ 0x26, {"LD H,n", a::IMM, &c::LDRN, 7} },
		{ 0x2E, {"LD L,n", a::IMM, &c::LDRN, 7} },

		{ 0x32, {"LD (nn),A", a::IMX, &c::LDNNA, 13} },

		{ 0x36, {"LD (HL),n", a::IMM, &c::LDHLN, 10} },

		{ 0x3A, {"LD A,(nn)", a::IMX, &c::LDANN, 13} },

		{ 0x3E, {"LD A,n", a::IMM, &c::LDRN, 7} },

		{ 0x40, {"LD B,B", a::REG, &c::LDRR, 4} },
		{ 0x41, {"LD B,C", a::REG, &c::LDRR, 4} },
		{ 0x42, {"LD B,D", a::REG, &c::LDRR, 4} },
		{ 0x43, {"LD B,E", a::REG, &c::LDRR, 4} },
		{ 0x44, {"LD B,H", a::REG, &c::LDRR, 4} },
		{ 0x45, {"LD B,L", a::REG, &c::LDRR, 4} },
		{ 0x46, {"LD B,(HL)", a::RGIHL, &c::LDRHL, 7} },
		{ 0x47, {"LD B,A", a::REG, &c::LDRR, 4} },
		{ 0x48, {"LD C,B", a::REG, &c::LDRR, 4} },
		{ 0x49, {"LD C,C", a::REG, &c::LDRR, 4} },
		{ 0x4A, {"LD C,D", a::REG, &c::LDRR, 4} },
		{ 0x4B, {"LD C,E", a::REG, &c::LDRR, 4} },
		{ 0x4C, {"LD C,H", a::REG, &c::LDRR, 4} },
		{ 0x4D, {"LD C,L", a::REG, &c::LDRR, 4} },
		{ 0x4E, {"LD C,(HL)",a::RGIHL, &c::LDRHL, 7} },
		{ 0x4F, {"LD C,A", a::REG, &c::LDRR, 4} },
		{ 0x50, {"LD D,B", a::REG, &c::LDRR, 4} },
		{ 0x51, {"LD D,C", a::REG, &c::LDRR, 4} },
		{ 0x52, {"LD D,D", a::REG, &c::LDRR, 4} },
		{ 0x53, {"LD D,E", a::REG, &c::LDRR, 4} },
		{ 0x54, {"LD D,H", a::REG, &c::LDRR, 4} },
		{ 0x55, {"LD D,L", a::REG, &c::LDRR, 4} },
		{ 0x56, {"LD D,(HL)", a::RGIHL, &c::LDRHL, 7} },
		{ 0x57, {"LD D,A", a::REG, &c::LDRR, 4} },
		{ 0x58, {"LD E,B", a::REG, &c::LDRR, 4} },
		{ 0x59, {"LD E,C", a::REG, &c::LDRR, 4} },
		{ 0x5A, {"LD E,D", a::REG, &c::LDRR, 4} },
		{ 0x5B, {"LD E,E", a::REG, &c::LDRR, 4} },
		{ 0x5C, {"LD E,H", a::REG, &c::LDRR, 4} },
		{ 0x5D, {"LD E,L", a::REG, &c::LDRR, 4} },
		{ 0x5E, {"LD E,(HL)", a::RGIHL, &c::LDRHL, 7} },
		{ 0x5F, {"LD E,A", a::REG, &c::LDRR, 4} },
		{ 0x60, {"LD H,B", a::REG, &c::LDRR, 4} },
		{ 0x61, {"LD H,C", a::REG, &c::LDRR, 4} },
		{ 0x62, {"LD H,D", a::REG, &c::LDRR, 4} },
		{ 0x63, {"LD H,E", a::REG, &c::LDRR, 4} },
		{ 0x64, {"LD H,H", a::REG, &c::LDRR, 4} },
		{ 0x65, {"LD H,L", a::REG, &c::LDRR, 4} },
		{ 0x66, {"LD H,(HL)", a::RGIHL, &c::LDRHL, 7} },
		{ 0x67, {"LD H,A", a::REG, &c::LDRR, 4} },
		{ 0x68, {"LD L,B", a::REG, &c::LDRR, 4} },
		{ 0x69, {"LD L,C", a::REG, &c::LDRR, 4} },
		{ 0x6A, {"LD L,D", a::REG, &c::LDRR, 4} },
		{ 0x6B, {"LD L,E", a::REG, &c::LDRR, 4} },
		{ 0x6C, {"LD L,H", a::REG, &c::LDRR, 4} },
		{ 0x6D, {"LD L,L", a::REG, &c::LDRR, 4} },
		{ 0x6E, {"LD L,(HL)", a::RGIHL, &c::LDRHL, 7} },
		{ 0x6F, {"LD L,A", a::REG, &c::LDRR, 4} },

		{ 0x70, {"LD (HL),B", a::IMP, &c::LDHLR, 7} },
		{ 0x71, {"LD (HL),C", a::IMP, &c::LDHLR, 7} },
		{ 0x72, {"LD (HL),D", a::IMP, &c::LDHLR, 7} },
		{ 0x73, {"LD (HL),E", a::IMP, &c::LDHLR, 7} },
		{ 0x74, {"LD (HL),H", a::IMP, &c::LDHLR, 7} },
		{ 0x75, {"LD (HL),L", a::IMP, &c::LDHLR, 7} },
		{ 0x76, {"HALT", a::IMP, &c::HALT, 4} },
		{ 0x77, {"LD (HL),A", a::IMP, &c::LDHLR, 7} },
		{ 0x78, {"LD A,B", a::REG, &c::LDRR, 4} },
		{ 0x79, {"LD A,C", a::REG, &c::LDRR, 4} },
		{ 0x7A, {"LD A,D", a::REG, &c::LDRR, 4} },
		{ 0x7B, {"LD A,E", a::REG, &c::LDRR, 4} },
		{ 0x7C, {"LD A,H", a::REG, &c::LDRR, 4} },
		{ 0x7D, {"LD A,L", a::REG, &c::LDRR, 4} },

		{ 0x7E, {"LD A,(HL)", a::RGIHL, &c::LDRHL, 7} },

		{ 0x7F, {"LD A,A", a::REG, &c::LDRR, 4} },

		{ 0xC6, {"ADD A,n", a::IMM, &c::ADDAN, 7} },

		{ 0xCE, {"ADC A,n", a::IMM, &c::ADCAN, 7} },

		//Multi-byte Opcode
		{ 0xCB, {"NOP", a::IMP, &c::NOP, 4} },
		{ 0xDD, {"NOP", a::IMP, &c::NOP, 4} },
		{ 0xED, {"NOP", a::IMP, &c::NOP, 4} },
		{ 0xFD, {"NOP", a::IMP, &c::NOP, 4} },
		
	};

	ddInstructions =
	{
		{ 0x36, {"LD (IX+d),n", a::REL, &c::LDIXDN, 19} },

		{ 0x46, {"LD B,(IX+d)", a::REL, &c::LDRIXD, 19} },
		{ 0x4E, {"LD C,(IX+d)", a::REL, &c::LDRIXD, 19} },
		{ 0x56, {"LD D,(IX+d)", a::REL, &c::LDRIXD, 19} },
		{ 0x5E, {"LD E,(IX+d)", a::REL, &c::LDRIXD, 19} },
		{ 0x66, {"LD H,(IX+d)", a::REL, &c::LDRIXD, 19} },
		{ 0x6E, {"LD L,(IX+d)", a::REL, &c::LDRIXD, 19} },

		{ 0x70, {"LD (IX+d),B", a::REL, &c::LDIXDR, 19} },
		{ 0x71, {"LD (IX+d),C", a::REL, &c::LDIXDR, 19} },
		{ 0x72, {"LD (IX+d),D", a::REL, &c::LDIXDR, 19} },
		{ 0x73, {"LD (IX+d),E", a::REL, &c::LDIXDR, 19} },
		{ 0x74, {"LD (IX+d),H", a::REL, &c::LDIXDR, 19} },
		{ 0x75, {"LD (IX+d),L", a::REL, &c::LDIXDR, 19} },

		{ 0x77, {"LD (IX+d),A", a::REL, &c::LDIXDR, 19} },

		{ 0x7E, {"LD A,(IX+d)", a::REL, &c::LDRIXD, 19} },
		
	};

	fdInstructions =
	{
		{ 0x36, {"LD (IY+d),n", a::REL, &c::LDIYDN, 19} },

		{ 0x46, {"LD B,(IY+d)", a::REL, &c::LDRIYD, 19} },
		{ 0x4E, {"LD C,(IY+d)", a::REL, &c::LDRIYD, 19} },
		{ 0x56, {"LD D,(IY+d)", a::REL, &c::LDRIYD, 19} },
		{ 0x5E, {"LD E,(IY+d)", a::REL, &c::LDRIYD, 19} },
		{ 0x66, {"LD H,(IY+d)", a::REL, &c::LDRIYD, 19} },
		{ 0x6E, {"LD L,(IY+d)", a::REL, &c::LDRIYD, 19} },

		{ 0x70, {"LD (IY+d),B", a::REL, &c::LDIYDR, 19} },
		{ 0x71, {"LD (IY+d),C", a::REL, &c::LDIYDR, 19} },
		{ 0x72, {"LD (IY+d),D", a::REL, &c::LDIYDR, 19} },
		{ 0x73, {"LD (IY+d),E", a::REL, &c::LDIYDR, 19} },
		{ 0x74, {"LD (IY+d),H", a::REL, &c::LDIYDR, 19} },
		{ 0x75, {"LD (IY+d),L", a::REL, &c::LDIYDR, 19} },

		{ 0x77, {"LD (IY+d),A", a::REL, &c::LDIYDR, 19} },

		{ 0x7E, {"LD A,(IY+d)", a::REL, &c::LDRIYD, 19} },
		
	};

	edInstructions =
	{
		{ 0x47, {"LD I,A", a::REG, &c::LDIA, 9}},
		{ 0x4F, {"LD R,A", a::REG, &c::LDRA, 9}},
		{ 0x57, {"LD A,I", a::REG, &c::LDAI, 9}},
		{ 0x5F, {"LD A,R", a::REG, &c::LDAR, 9}},

	};

	cbInstructions =
	{


	};

}