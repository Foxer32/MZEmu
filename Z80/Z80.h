#pragma once
#include "Device.h"
#include "IClockable.h"
#include <vector>

class Z80 : public Device, public IClockable
{
public:

    Z80();
    ~Z80();

    uint8_t A, F, B, C, D, E, H, L;
    uint8_t A1, F1, B1, C1, D1, E1, H1, L1;

    uint8_t I, R;
    uint16_t IX, IY, IR;
    uint16_t SP = 0x0000;
    uint16_t PC = 0x0000;

    uint16_t MEMPTR = 0x0000;
    uint8_t Q = 0x00;

    uint8_t interruptMode = 0;

    bool IFF1;
    bool IFF2;
    bool isHalted;
    bool irIsIX;

    void setCpuFrequency(int frequency);

    virtual int step() override;

    void reset(bool hardReset = false);

    void maskableInterrupt();
    void nonMaskableInterrupt();
    std::vector <uint8_t> interruptData;

private:

    enum class AddressingModes
    {
        IMP = 0,
        IDX = 1,
        IMM = 2,
        REL = 3,
        RELS = 4,
        IMX = 5,
        REG = 6,
        RGIHL = 7,
    };

    enum class RegisterPairs
    {
        AF = 0,
        HL = 1,
        BC = 2,
        DE = 3,
        AF1 = 4,
        HL1 = 5,
        BC1 = 6,
        DE1 = 7,
    };

    enum class Flags
    {
        C = 1 << 0,
        N = 1 << 1,
        P = 1 << 2,
        X = 1 << 3,
        H = 1 << 4,
        U = 1 << 5,
        Z = 1 << 6,
        S = 1 << 7,
    };

    struct Instruction
    {
        const char* mnemonic;
        AddressingModes addressingMode;
        uint8_t(Z80::* op)(void);
        uint8_t tCycles;
    };

    std::vector <Instruction> rootInstructions;
    std::vector <Instruction> edInstructions;
    std::vector <Instruction> cbInstructions;
    std::vector <Instruction> irInstructions;
    std::vector <Instruction> ircbInstructions;

    bool genINT, genNMI;

    uint8_t currentOpCode = 0x00;
    uint16_t absoluteAddress = 0x0000;
    Instruction* currentInstruction;
    uint8_t clockCycles = 0;
    uint8_t refresh = 0;

    void initInstructions();

    void incrementRefreshRegister(uint8_t steps);
    void unhaltIfHalted();
    void handleInterrupts();
    void modInstructionIfInterrupt();

    //Helpers 1
    void setQ();
    void resetQ();
    bool getFlag(Flags flag);
    void setFlag(Flags flag, bool v);
    uint8_t readMemoryNext();
    uint16_t readMemoryNext2Bytes();
	uint8_t readMemory(uint16_t addr);
	void writeMemory(uint16_t addr, uint8_t data);
	uint8_t readPeripheral(uint16_t addr);
	void writePeripheral(uint16_t addr, uint8_t data);
    void incRegisterPair(RegisterPairs rp, int16_t v = 1);
    uint16_t readRegisterPair(RegisterPairs src);
    void writeRegisterPair(RegisterPairs dest, uint16_t v);
    uint8_t readFromRegister(uint8_t src);
    void writeToRgister(uint8_t dest, uint8_t v);
    //Helpers 2
    void writeDD(uint16_t v, uint8_t dd);
    uint8_t add8(uint8_t a, uint8_t b, bool c = false);
    uint8_t sub8(uint8_t a, uint8_t b, bool c = false);
    bool parity(uint16_t v);
    void andAN(uint8_t n);
    void orAN(uint8_t n);
    void xorAN(uint8_t n);
    void compareAN(uint8_t n);
    uint8_t incN(uint8_t n);
    uint8_t decN(uint8_t n);
    uint16_t readRegisterPair2(uint8_t src, uint16_t self = 0, bool haveSelf = false);
    void writeRegisterPair2(uint8_t dest, uint16_t v);
    uint16_t add16(uint16_t a, uint16_t b, bool c = false, bool withCarry = false);
    uint16_t sub16(uint16_t a, uint16_t b, bool c = false);
    void setRotateFlags(uint8_t n, bool resetS = false);
    bool evaluateCC(uint8_t cc);
    void pushPC();
    void popPC();
    void setUndocIOFlags(uint16_t k, uint8_t n);
    void saveIR();
    //Helpers 3
    uint8_t LDCPr(bool c);
    uint8_t JRif(bool c);
    uint8_t IOr();

    //Missing
    uint8_t XXX();

    //Load8
    uint8_t LDRR();
    uint8_t LDRN();
    uint8_t LDRHL();
    uint8_t LDRIRD();
    uint8_t LDHLR();
    uint8_t LDIRDR();
    uint8_t LDHLN();
    uint8_t LDIRDN();
    uint8_t LDABC();
    uint8_t LDADE();
    uint8_t LDANN();
    uint8_t LDBCA();
    uint8_t LDDEA();
    uint8_t LDNNA();
    uint8_t LDAI();
    uint8_t LDAR();
    uint8_t LDIA();
    uint8_t LDRA();

    //Load16
    uint8_t LDDDNN();
    uint8_t LDIRNN();
    uint8_t LDHLNN();
    uint8_t LDDDFNN();
    uint8_t LDIRFNN();
    uint8_t LDNNHL();
    uint8_t LDNNDD();
    uint8_t LDNNIR();
    uint8_t LDSPHL();
    uint8_t LDSPIR();
    uint8_t PUSHQQ();
    uint8_t PUSHIR();
    uint8_t POPQQ();
    uint8_t POPIR();
    //Undocumented
    uint8_t LDIRHN();
    uint8_t LDIRLN();
    uint8_t LDRIRH();
    uint8_t LDRIRL();
    uint8_t LDIRHR();
    uint8_t LDIRLR();
    uint8_t LDIRHIRH();
    uint8_t LDIRLIRL();
    uint8_t LDIRHIRL();
    uint8_t LDIRLIRH();

    //Exchange
    uint8_t EXDEHL();
    uint8_t EXAFAF();
    uint8_t EXX();
    uint8_t EXSPHL();
    uint8_t EXSPIR();
    uint8_t LDI();
    uint8_t LDIR();
    uint8_t LDD();
    uint8_t LDDR();
    uint8_t CPI();
    uint8_t CPIR();
    uint8_t CPD();
    uint8_t CPDR();

    //Arithmetic8
    uint8_t ADDAR();
    uint8_t ADDAN();
    uint8_t ADDAHL();
    uint8_t ADDAIRD();
    uint8_t ADCAR();
    uint8_t ADCAN();
    uint8_t ADCAHL();
    uint8_t ADCAIRD();
    uint8_t SUBR();
    uint8_t SUBN();
    uint8_t SUBHL();
    uint8_t SUBIRD();
    uint8_t SBCAR();
    uint8_t SBCAN();
    uint8_t SBCAHL();
    uint8_t SBCAIRD();
    uint8_t ANDR();
    uint8_t ANDN();
    uint8_t ANDHL();
    uint8_t ANDIRD();
    uint8_t ORR();
    uint8_t ORN();
    uint8_t ORHL();
    uint8_t ORIRD();
    uint8_t XORR();
    uint8_t XORN();
    uint8_t XORHL();
    uint8_t XORIRD();
    uint8_t CPR();
    uint8_t CPN();
    uint8_t CPHL();
    uint8_t CPIRD();
    uint8_t INCR();
    uint8_t INCHL();
    uint8_t INCIRD();
    uint8_t DECR();
    uint8_t DECHL();
    uint8_t DECIRD();

    //General
    uint8_t DAA();
    uint8_t CPL();
    uint8_t NEG();
    uint8_t CCF();
    uint8_t SCF();
    uint8_t NOP();
    uint8_t HALT();
    uint8_t DI();
    uint8_t EI();
    uint8_t IM0();
    uint8_t IM1();
    uint8_t IM2();

    //Arithmetic16
    uint8_t ADDHLSS();
    uint8_t ADCHLSS();
    uint8_t SBCHLSS();
    uint8_t ADDIRPP();
    uint8_t INCSS();
    uint8_t INCIR();
    uint8_t DECSS();
    uint8_t DECIR();
    //Undocumented
    uint8_t INCIRH();
    uint8_t INCIRL();
    uint8_t DECIRH();
    uint8_t DECIRL();
    uint8_t ADDAIRH();
    uint8_t ADDAIRL();
    uint8_t ADCAIRH();
    uint8_t ADCAIRL();
    uint8_t SUBAIRH();
    uint8_t SUBAIRL();
    uint8_t SBCAIRH();
    uint8_t SBCAIRL();
    uint8_t ANDIRH();
    uint8_t ANDIRL();
    uint8_t XORIRH();
    uint8_t XORIRL();
    uint8_t ORIRH();
    uint8_t ORIRL();
    uint8_t CPIRH();
    uint8_t CPIRL();

    //RotateShift
    uint8_t RLCA();
    uint8_t RLA();
    uint8_t RRCA();
    uint8_t RRA();
    uint8_t RLCR();
    uint8_t RLCHL();
    uint8_t RLCIRD();
    uint8_t RLR();
    uint8_t RLHL();
    uint8_t RLIRD();
    uint8_t RRCR();
    uint8_t RRCHL();
    uint8_t RRCIRD();
    uint8_t RRR();
    uint8_t RRHL();
    uint8_t RRIRD();
    uint8_t SLAR();
    uint8_t SLAHL();
    uint8_t SLAIRD();
    uint8_t SRAR();
    uint8_t SRAHL();
    uint8_t SRAIRD();
    uint8_t SRLR();
    uint8_t SRLHL();
    uint8_t SRLIRD();
    uint8_t RLD();
    uint8_t RRD();
    //Undocumented
    uint8_t SLSR();
    uint8_t SLSHL();
    uint8_t SLSIRD();

    //Bit
    uint8_t BITBR();
    uint8_t BITBHL();
    uint8_t BITBIRD();
    uint8_t SETBR();
    uint8_t SETBHL();
    uint8_t SETBIRD();
    uint8_t RESBR();
    uint8_t RESBHL();
    uint8_t RESBIRD();

    //Jump
    uint8_t JPNN();
    uint8_t JPCCNN();
    uint8_t JRE();
    uint8_t JRCE();
    uint8_t JRNCE();
    uint8_t JRZE();
    uint8_t JRNZE();
    uint8_t JPHL();
    uint8_t JPIR();
    uint8_t DJNZE();

    //CallReturn
    uint8_t CALLNN();
    uint8_t CALLCCNN();
    uint8_t RET();
    uint8_t RETCC();
    uint8_t RETI();
    uint8_t RETN();
    uint8_t RSTP();

    //InputOutput
    uint8_t INAN();
    uint8_t INRC();
    uint8_t INI();
    uint8_t INIR();
    uint8_t IND();
    uint8_t INDR();
    uint8_t OUTNA();
    uint8_t OUTCR();
    uint8_t OUTI();
    uint8_t OTIR();
    uint8_t OUTD();
    uint8_t OTDR();

};
