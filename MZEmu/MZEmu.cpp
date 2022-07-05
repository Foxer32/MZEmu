#include <iostream>
#include <string> 
#include <cstdint>
#include "Bus.h"
//using namespace std;

Bus bus;

int main()
{
    bus.writeMemory(0x7FFB, 0x00);
    bus.writeMemory(0x7FFC, 0x00);
    bus.writeMemory(0x7FFD, 0x00);
    bus.writeMemory(0x7FFE, 0x00);
    bus.writeMemory(0x7FFF, 0x00);
    bus.writeMemory(0x8000, 0x76); //----------
    bus.writeMemory(0x8001, 0x44);
    bus.writeMemory(0x8002, 0xA2);
    bus.writeMemory(0x8003, 0xFA);
    bus.writeMemory(0x8004, 0x00);
    bus.writeMemory(0x8005, 0x00);

    bus.writeMemory(0xB3EA, 0x6C);

    bus.writeMemory(0xFA20, 0xA5);
    bus.writeMemory(0xFA21, 0xCB);

    bus.cpu.H = 0xB3;
    bus.cpu.L = 0xEA;
    bus.cpu.SP = bus.cpu.IX = bus.cpu.IY = 0xB448;
    bus.cpu.PC = 0x8000;
    bus.cpu.A = 0x6C;

    std::cout << bus.cpu.getCpuInfo();

    while(true) {
        system("pause");

        //bus.cpu.tick();
        bus.cpu.step();
        std::cout << bus.cpu.getCpuInfo();       
    }

    return 0;
}

