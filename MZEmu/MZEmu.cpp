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
    bus.writeMemory(0x8000, 0xC6); //----------
    bus.writeMemory(0x8001, 0xF2);
    bus.writeMemory(0x8002, 0x00);
    bus.writeMemory(0x8003, 0x00);
    bus.writeMemory(0x8004, 0x00);
    bus.writeMemory(0x8005, 0x00);

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

