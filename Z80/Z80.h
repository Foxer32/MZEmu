#pragma once
#include "Device.h"
#include <string>
#include <map>

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
        //X = 1 << 3,
        H = 1 << 4,
        //U = 1 << 5,
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

    //uint16_t MEMPTR = 0x0000;
    //uint8_t Q = 0x00;

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

    std::map<uint8_t, Instruction> rootInstructions;
    std::map<uint8_t, Instruction> ddInstructions;
    std::map<uint8_t, Instruction> fdInstructions;
    std::map<uint8_t, Instruction> edInstructions;
    std::map<uint8_t, Instruction> cbInstructions;

    uint16_t absoluteAddress = 0x0000;
    uint8_t currentOpCode = 0x00;
    Instruction* currentInstruction;
    uint8_t clockCycles = 0;

    //void setQ();
    //void resetQ();

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

    uint8_t readMemoryNext();
    void initInstructions();

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
    
    //ExchangeBlockTransferAndSearch

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

    //RotateAndShift

    //Bit

    //Jump

    //CallReturn

    //InputOutput
    


};

