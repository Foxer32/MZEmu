#include "CPMBasedTester.h"

CPMBasedTester::CPMBasedTester()
{

}

CPMBasedTester::~CPMBasedTester()
{

}

void CPMBasedTester::runTests()
{
	runTest("tests/prelim.com", 8721);
    runTest("tests/zexdoc.com", 46734978649);
    runTest("tests/zexall.com", 46734978649);

    system("pause");
}

void CPMBasedTester::runTest(string fileName, uint64_t expectedTStates)
{
    cout << "*** Test: " << fileName << " started\n";

    for (auto& i : bus.mem) i = 0x00;
	loadFile(fileName, 0x100);

    bus.cpu.PC = 0x100;

    bus.mem[0x0000] = 0xD3;
    bus.mem[0x0001] = 0x00;

    bus.mem[0x0005] = 0xDB;
    bus.mem[0x0006] = 0x00;
    bus.mem[0x0007] = 0xC9;

    uint64_t totalTStates = 0;
    uint64_t instructionsExequted = 0;

    while (!bus.testFinished)
    {
        instructionsExequted++;
        totalTStates += bus.cpu.step();
    }

    bus.testFinished = false;

    int64_t diff = expectedTStates - totalTStates;
    cout << "\n*** Test " << fileName << " finished\n*** " << to_string(instructionsExequted)
        << " instructions executed on " << to_string(totalTStates) << " T States (expected: "
        << to_string(expectedTStates) << " diff: " << to_string(diff) << " )\n\n";
}

void CPMBasedTester::loadFile(string fileName, uint16_t addr)
{   
    std::ifstream ifs;
    ifs.open(fileName, std::ifstream::binary);
    if (ifs.is_open())
    {
        ifs.seekg(0, ios::end);
        auto end = ifs.tellg();
        ifs.seekg(0, ios::beg);

        ifs.read((char*)&bus.mem[addr], end);
        ifs.close();
    }
}
