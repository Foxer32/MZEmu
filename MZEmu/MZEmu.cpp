#include <iostream>
#include <string> 
#include <cstdint>
#include "Bus.h"
//using namespace std;

Bus bus;

int main()
{

    bus.writeMemory(0x0000, 0xC6);
    bus.writeMemory(0x0001, 0xF2);
    bus.writeMemory(0x0002, 0xF2);
    bus.writeMemory(0x0003, 0x0A);

    bus.cpu.A = 0x6C;
    //while(true)
        bus.cpu.tick();
    
    //std::cout << "data:" << std::to_string(bus.readMemory(0xFFF0)) << "\n";*/

    return 0;
}

