#include "Z80.h"

template<class T>
std::string Z80::tostringX(T v)
{
    uint8_t len = sizeof(T) * 2;

    std::string str(len, ' ');

    for (int16_t i = len - 1; i >= 0; i--, v >>= 4)
    {
        str[i] = "0123456789ABCDEF"[v & 0xF];
    }

    return str;
}

template<class T>
std::string Z80::tostringB( T v)
{
    uint8_t len = sizeof(T) * 8;

    std::string str(len, ' ');

    for (int16_t i = len - 1; i >= 0; i--, v >>= 1)
    {
        str[i] = (v & 1) ? '1' : '0';
    }

    return str;
}

//This terrible function will be removed later
std::string Z80::getCpuInfo()
{
    std::string buff = " A = " + tostringX(A) + "\t B = " + tostringX(B) + "\t C = " + tostringX(C) + "\t D = " + tostringX(D) + "\t E = " + tostringX(E) + "\t H = " + tostringX(H) + "\t L = " + tostringX(L) + '\n';
    buff += "A1 = " + tostringX(A1) + "\tB1 = " + tostringX(B1) + "\tC1 = " + tostringX(C1) + "\tD1 = " + tostringX(D1) + "\tE1 = " + tostringX(E1) + "\tH1 = " + tostringX(H1) + "\tL1 = " + tostringX(L1) + '\n';
    buff += "\n F = " + tostringB(F) + "\tF1 = " + tostringB(F1) + "\tIX = " + tostringX(IX) + "    PC = " + tostringX(PC) + "\n     SZUHXPNC\t     SZUHXPNC\tIY = " + tostringX(IY) + "    SP = " + tostringX(SP) + "\n";
    buff += "\n I = " + tostringX(I) + "\t R = " + tostringX(R);
    buff += "\n\nCurrent(last) instruction: " + ( (&currentInstruction->mnemonic == NULL) ? "???" : currentInstruction->mnemonic ) + "\tClockCycles: " + std::to_string(clockCycles) + "\n\n";

    return buff;
}
