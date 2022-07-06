#pragma once
#include "Device.h"
#include <string>
#include <vector>

class Z80 : public Device
{
public:

    Z80();
    ~Z80();

    uint8_t A, F, B, C, D, E, H, L;
    uint8_t A1, F1, B1, C1, D1, E1, H1, L1;

    uint8_t I, R;
    uint16_t IX, IY;
    uint16_t SP = 0x0000;
    uint16_t PC = 0x0000;

    bool IFF1;
    bool IFF2;
    bool isHalted;

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

    enum class InterruptModes
    {
        Mode0 = 0,
        Mode1 = 1,
        Mode2 = 2,
    };

    InterruptModes interruptMode = InterruptModes::Mode0;

    uint16_t MEMPTR = 0x0000;
    uint8_t Q = 0x00;

    void tick();
    void step();
    void reset(bool hardReset = false);

    template<class T>
    std::string tostringX(T v);
    template<class T>
    std::string tostringB(T v);
    std::string getCpuInfo();

private:

    struct Instruction
    {
        std::string mnemonic;
        AddressingModes addressingMode;
        uint8_t(Z80::* op)(void);
        uint8_t tCycles;
    };

    std::vector <Instruction> rootInstructions;
    std::vector <Instruction> ddInstructions;
    std::vector <Instruction> fdInstructions;
    std::vector <Instruction> edInstructions;
    std::vector <Instruction> cbInstructions;
    std::vector <Instruction> ddcbInstructions;
    std::vector <Instruction> fdcbInstructions;

    uint16_t absoluteAddress = 0x0000;
    uint8_t currentOpCode = 0x00;
    Instruction* currentInstruction;
    uint8_t clockCycles = 0;

    void setQ();
    void resetQ();

    bool getFlag(Flags flag);
    void setFlag(Flags flag, bool v);

	uint8_t readMemory(uint16_t addr);
	void writeMemory(uint16_t addr, uint8_t data);
	uint8_t readPeripheral(uint16_t addr);
	void writePeripheral(uint16_t addr, uint8_t data);
   
    uint16_t readRegisterPair(RegisterPairs src);
    void writeRegisterPair(RegisterPairs dest, uint16_t v);
    uint8_t readFromRegister(uint8_t src);
    void writeToRgister(uint8_t dest, uint8_t v);

    void incRegisterPair(RegisterPairs rp, int16_t v = 1);

    uint8_t readMemoryNext();
    void initInstructions();

    //Helpers
    void writeDD(uint16_t v, uint8_t dd);
    uint8_t add8(uint8_t a, uint8_t b, uint8_t c = 0);
    uint8_t sub8(uint8_t a, uint8_t b, uint8_t c = 0);
    bool parity(uint16_t v);
    uint8_t andAB(uint8_t a,uint8_t b);
    uint8_t orAB(uint8_t a, uint8_t b);
    uint8_t xorAB(uint8_t a, uint8_t b);
    void setComparsionFlags(uint8_t n, uint8_t diff);
    void setIncFlags(uint8_t val, uint8_t incVal);
    void setDecFlags(uint8_t val, uint8_t decVal);
    uint16_t readRegisterPair2(uint8_t src, uint16_t self = 0, bool haveSelf = false);
    void writeRegisterPair2(uint8_t dest, uint16_t v);
    uint16_t add16(uint16_t a, uint16_t b, uint8_t c = 0, bool withCarry = false);
    uint16_t sub16(uint16_t a, uint16_t b, uint8_t c = 0);
    void setRotateFlags(uint8_t n, bool resetS = false);

    //Missing
    uint8_t XXX();

    //Load8
    uint8_t LDRR();
    uint8_t LDRN();
    uint8_t LDRHL();
    uint8_t LDRIXD();
    uint8_t LDRIYD();
    uint8_t LDHLR();
    uint8_t LDIXDR();
    uint8_t LDIYDR();
    uint8_t LDHLN();
    uint8_t LDIXDN();
    uint8_t LDIYDN();
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
    uint8_t LDIXNN();
    uint8_t LDIYNN();
    uint8_t LDHLNN();
    uint8_t LDDDFNN();
    uint8_t LDIXFNN();
    uint8_t LDIYFNN();
    uint8_t LDNNHL();
    uint8_t LDNNDD();
    uint8_t LDNNIX();
    uint8_t LDNNIY();
    uint8_t LDSPHL();
    uint8_t LDSPIX();
    uint8_t LDSPIY();
    uint8_t PUSHQQ();
    uint8_t PUSHIX();
    uint8_t PUSHIY();
    uint8_t POPQQ();
    uint8_t POPIX();
    uint8_t POPIY();
    //Undocumented
    uint8_t LDIXHN();
    uint8_t LDIYHN();
    uint8_t LDIXLN();
    uint8_t LDIYLN();
    uint8_t LDRIXH();
    uint8_t LDRIYH();
    uint8_t LDRIXL();
    uint8_t LDRIYL();
    uint8_t LDIXHR();
    uint8_t LDIYHR();
    uint8_t LDIXLR();
    uint8_t LDIYLR();
    uint8_t LDIXHIXH();
    uint8_t LDIXLIXL();
    uint8_t LDIXHIXL();
    uint8_t LDIXLIXH();
    uint8_t LDIYHIYH();
    uint8_t LDIYLIYL();
    uint8_t LDIYHIYL();
    uint8_t LDIYLIYH();

    //Exchange
    uint8_t EXDEHL();
    uint8_t EXAFAF();
    uint8_t EXX();
    uint8_t EXSPHL();
    uint8_t EXSPIX();
    uint8_t EXSPIY();
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
    uint8_t ADDAIXD();
    uint8_t ADDAIYD();
    uint8_t ADCAR();
    uint8_t ADCAN();
    uint8_t ADCAHL();
    uint8_t ADCAIXD();
    uint8_t ADCAIYD();
    uint8_t SUBR();
    uint8_t SUBN();
    uint8_t SUBHL();
    uint8_t SUBIXD();
    uint8_t SUBIYD();
    uint8_t SBCAR();
    uint8_t SBCAN();
    uint8_t SBCAHL();
    uint8_t SBCAIXD();
    uint8_t SBCAIYD();
    uint8_t ANDR();
    uint8_t ANDN();
    uint8_t ANDHL();
    uint8_t ANDIXD();
    uint8_t ANDIYD();
    uint8_t ORR();
    uint8_t ORN();
    uint8_t ORHL();
    uint8_t ORIXD();
    uint8_t ORIYD();
    uint8_t XORR();
    uint8_t XORN();
    uint8_t XORHL();
    uint8_t XORIXD();
    uint8_t XORIYD();
    uint8_t CPR();
    uint8_t CPN();
    uint8_t CPHL();
    uint8_t CPIXD();
    uint8_t CPIYD();
    uint8_t INCR();
    uint8_t INCHL();
    uint8_t INCIXD();
    uint8_t INCIYD();
    uint8_t DECR();
    uint8_t DECHL();
    uint8_t DECIXD();
    uint8_t DECIYD();

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
    uint8_t ADDIXPP();
    uint8_t ADDIYRR();
    uint8_t INCSS();
    uint8_t INCIX();
    uint8_t INCIY();
    uint8_t DECSS();
    uint8_t DECIX();
    uint8_t DECIY();
    //Undocumented
    uint8_t INCIXH();
    uint8_t INCIXL();
    uint8_t INCIYH();
    uint8_t INCIYL();
    uint8_t DECIXH();
    uint8_t DECIYH();
    uint8_t DECIXL();
    uint8_t DECIYL();
    uint8_t ADDAIXH();
    uint8_t ADDAIXL();
    uint8_t ADDAIYH();
    uint8_t ADDAIYL();
    uint8_t ADCAIXH();
    uint8_t ADCAIXL();
    uint8_t ADCAIYH();
    uint8_t ADCAIYL();
    uint8_t SUBAIXH();
    uint8_t SUBAIXL();
    uint8_t SUBAIYH();
    uint8_t SUBAIYL();
    uint8_t SBCAIXH();
    uint8_t SBCAIXL();
    uint8_t SBCAIYH();
    uint8_t SBCAIYL();
    uint8_t ANDIXH();
    uint8_t ANDIXL();
    uint8_t ANDIYH();
    uint8_t ANDIYL();
    uint8_t XORIXH();
    uint8_t XORIXL();
    uint8_t XORIYH();
    uint8_t XORIYL();
    uint8_t ORIXH();
    uint8_t ORIXL();
    uint8_t ORIYH();
    uint8_t ORIYL();
    uint8_t CPIXH();
    uint8_t CPIXL();
    uint8_t CPIYH();
    uint8_t CPIYL();

    //RotateShift
    uint8_t RLCA();
    uint8_t RLA();
    uint8_t RRCA();
    uint8_t RRA();
    uint8_t RLCR();
    uint8_t RLCHL();
    uint8_t RLCIXD();
    uint8_t RLCIYD();
    uint8_t RLR();
    uint8_t RLHL();
    uint8_t RLIXD();
    uint8_t RLIYD();
    uint8_t RRCR();
    uint8_t RRCHL();
    uint8_t RRCIXD();
    uint8_t RRCIYD();
    uint8_t RRR();
    uint8_t RRHL();
    uint8_t RRIXD();
    uint8_t RRIYD();
    uint8_t SLAR();
    uint8_t SLAHL();
    uint8_t SLAIXD();
    uint8_t SLAIYD();
    uint8_t SRAR();
    uint8_t SRAHL();
    uint8_t SRAIXD();
    uint8_t SRAIYD();
    uint8_t SRLR();
    uint8_t SRLHL();
    uint8_t SRLIXD();
    uint8_t SRLIYD();
    uint8_t RLD();
    uint8_t RRD();
    //Undocumented
    uint8_t SLSR();
    uint8_t SLSHL();
    uint8_t SLSIXD();
    uint8_t SLSIYD();

    //Bit
    uint8_t BITBR();//TODO later
    uint8_t BITBHL();//TODO later //BITHL
    uint8_t BITBIXD();//TODO later //BITIXD
    uint8_t BITBIYD();//TODO later //BITIYD
    uint8_t SETBR();//TODO later
    uint8_t SETBHL();//TODO later //SETHL
    uint8_t SETBIXD();//TODO later //SETIXD
    uint8_t SETBIYD();//TODO later //SETIYD
    uint8_t RESBR();//TODO later
    uint8_t RESBHL();//TODO later //RESHL
    uint8_t RESBIXD();//TODO later //RESIXD
    uint8_t RESBIYD();//TODO later //RESIYD

    //Jump
    uint8_t JPNN();//TODO later
    uint8_t JPCCNN();//TODO later
    uint8_t JRE();//TODO later //JR
    uint8_t JRCE();//TODO later //JRC
    uint8_t JRNCE();//TODO later //JRNC
    uint8_t JRZE();//TODO later //JRZ
    uint8_t JRNZE();//TODO later //JRNZ
    uint8_t JPHL();//TODO later
    uint8_t JPIX();//TODO later
    uint8_t JPIY();//TODO later
    uint8_t DJNZE();//TODO later //DJNZ

    //CallReturn
    uint8_t CALLNN();//TODO later //CALL
    uint8_t CALLCCNN();//TODO later //CALLCC
    uint8_t RET();//TODO later
    uint8_t RETCC();//TODO later
    uint8_t RETI();//TODO later
    uint8_t RETN();//TODO later
    uint8_t RSTP();//TODO later //RST

    //InputOutput
    uint8_t INAN();//TODO later //INA
    uint8_t INRC();//TODO later //INR
    uint8_t INI();//TODO later
    uint8_t INIR();//TODO later
    uint8_t IND();//TODO later
    uint8_t INDR();//TODO later
    uint8_t OUTNA();//TODO later //OUTA
    uint8_t OUTCR();//TODO later //OUTR
    uint8_t OUTI();//TODO later
    uint8_t OTIR();//TODO later
    uint8_t OUTD();//TODO later
    uint8_t OTDR();//TODO later

};
