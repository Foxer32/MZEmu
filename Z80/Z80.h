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
    uint8_t ADDAR();//TODO later
    uint8_t ADDAN();
    uint8_t ADDAHL();//TODO later
    uint8_t ADDAIXD();//TODO later //ADDAIXDN
    uint8_t ADDAIYD();//TODO later //ADDAIYDN
    uint8_t ADCAR();//TODO later
    uint8_t ADCAN();
    uint8_t ADCAHL();//TODO later
    uint8_t ADCAIXD();//TODO later //ADCAIXDN
    uint8_t ADCAIYD();//TODO later //ADCAIYDN
    uint8_t SUBR();//TODO later //SUBAR
    uint8_t SUBN();//TODO later //SUBAN
    uint8_t SUBHL();//TODO later //SUBAHL
    uint8_t SUBIXD();//TODO later //SUBAIXDN
    uint8_t SUBIYD();//TODO later //SUBAIYDN
    uint8_t SBCAR();//TODO later
    uint8_t SBCAN();//TODO later
    uint8_t SBCAHL();//TODO later
    uint8_t SBCAIXD();//TODO later //SBCAIXDN
    uint8_t SBCAIYD();//TODO later //SBCAIYDN
    uint8_t ANDR();//TODO later
    uint8_t ANDN();//TODO later
    uint8_t ANDHL();//TODO later
    uint8_t ANDIXD();//TODO later
    uint8_t ANDIYD();//TODO later
    uint8_t ORR();//TODO later
    uint8_t ORN();//TODO later
    uint8_t ORHL();//TODO later
    uint8_t ORIXD();//TODO later
    uint8_t ORIYD();//TODO later
    uint8_t XORR();//TODO later
    uint8_t XORN();//TODO later
    uint8_t XORHL();//TODO later
    uint8_t XORIXD();//TODO later
    uint8_t XORIYD();//TODO later
    uint8_t CPR();//TODO later
    uint8_t CPN();//TODO later
    uint8_t CPHL();//TODO later
    uint8_t CPIXD();//TODO later
    uint8_t CPIYD();//TODO later
    uint8_t INCR();//TODO later
    uint8_t INCHL();//TODO later
    uint8_t INCIXD();//TODO later
    uint8_t INCIYD();//TODO later
    uint8_t DECR();//TODO later
    uint8_t DECHL();//TODO later
    uint8_t DECIXD();//TODO later
    uint8_t DECIYD();//TODO later

    //General
    uint8_t DAA();//TODO later
    uint8_t CPL();//TODO later
    uint8_t NEG();//TODO later
    uint8_t CCF();//TODO later
    uint8_t SCF();//TODO later
    uint8_t NOP();
    uint8_t HALT();//TODO later
    uint8_t DI();//TODO later
    uint8_t EI();//TODO later
    uint8_t IM0();//TODO later
    uint8_t IM1();//TODO later
    uint8_t IM2();//TODO later

    //Arithmetic16
    uint8_t ADDHLSS();//TODO later
    uint8_t ADCHLSS();//TODO later
    uint8_t SBCHLSS();//TODO later
    uint8_t ADDIXPP();//TODO later
    uint8_t ADDIYRR();//TODO later //ADDIYPP
    uint8_t INCSS();//TODO later
    uint8_t INCIX();//TODO later
    uint8_t INCIY();//TODO later
    uint8_t DECSS();//TODO later
    uint8_t DECIX();//TODO later
    uint8_t DECIY();//TODO later
    //Undocumented
    uint8_t INCIXH();//TODO later
    uint8_t INCIXL();//TODO later
    uint8_t INCIYH();//TODO later
    uint8_t INCIYL();//TODO later
    uint8_t DECIXH();//TODO later
    uint8_t DECIYH();//TODO later
    uint8_t DECIXL();//TODO later
    uint8_t DECIYL();//TODO later
    uint8_t ADDAIXH();//TODO later
    uint8_t ADDAIXL();//TODO later
    uint8_t ADDAIYH();//TODO later
    uint8_t ADDAIYL();//TODO later
    uint8_t ADCAIXH();//TODO later
    uint8_t ADCAIXL();//TODO later
    uint8_t ADCAIYH();//TODO later
    uint8_t ADCAIYL();//TODO later
    uint8_t SUBAIXH();//TODO later
    uint8_t SUBAIXL();//TODO later
    uint8_t SUBAIYH();//TODO later
    uint8_t SUBAIYL();//TODO later
    uint8_t SBCAIXH();//TODO later
    uint8_t SBCAIXL();//TODO later
    uint8_t SBCAIYH();//TODO later
    uint8_t SBCAIYL();//TODO later
    uint8_t ANDIXH();//TODO later
    uint8_t ANDIXL();//TODO later
    uint8_t ANDIYH();//TODO later
    uint8_t ANDIYL();//TODO later
    uint8_t XORIXH();//TODO later
    uint8_t XORIXL();//TODO later
    uint8_t XORIYH();//TODO later
    uint8_t XORIYL();//TODO later
    uint8_t ORIXH();//TODO later
    uint8_t ORIXL();//TODO later
    uint8_t ORIYH();//TODO later
    uint8_t ORIYL();//TODO later
    uint8_t CPIXH();//TODO later
    uint8_t CPIXL();//TODO later
    uint8_t CPIYH();//TODO later
    uint8_t CPIYL();//TODO later

    //RotateShift
    uint8_t RLCA();//TODO later
    uint8_t RLA();//TODO later
    uint8_t RRCA();//TODO later
    uint8_t RRA();//TODO later
    uint8_t RLCR();//TODO later
    uint8_t RLCHL();//TODO later
    uint8_t RLCIXD();//TODO later
    uint8_t RLCIYD();//TODO later
    uint8_t RLR();//TODO later
    uint8_t RLHL();//TODO later
    uint8_t RLIXD();//TODO later
    uint8_t RLIYD();//TODO later
    uint8_t RRCR();//TODO later
    uint8_t RRCHL();//TODO later
    uint8_t RRCIXD();//TODO later
    uint8_t RRCIYD();//TODO later
    uint8_t RRR();//TODO later
    uint8_t RRHL();//TODO later
    uint8_t RRIXD();//TODO later
    uint8_t RRIYD();//TODO later
    uint8_t SLAR();//TODO later
    uint8_t SLAHL();//TODO later
    uint8_t SLAIXD();//TODO later
    uint8_t SLAIYD();//TODO later
    uint8_t SRAR();//TODO later
    uint8_t SRAHL();//TODO later
    uint8_t SRAIXD();//TODO later
    uint8_t SRAIYD();//TODO later
    uint8_t SRLR();//TODO later
    uint8_t SRLHL();//TODO later
    uint8_t SRLIXD();//TODO later
    uint8_t SRLIYD();//TODO later
    uint8_t RLD();//TODO later
    uint8_t RRD();//TODO later
    //Undocumented
    uint8_t SLSR();//TODO later
    uint8_t SLSHL();//TODO later
    uint8_t SLSIXD();//TODO later
    uint8_t SLSIYD();//TODO later

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
