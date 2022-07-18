#pragma once
#include <fstream>
#include <string>
#include <iostream>

#include "Z80.h"
#include "TestBus.h"

using namespace std;

class CPMBasedTester
{
public:

	CPMBasedTester();
	~CPMBasedTester();

	void runTests();

private:

	TestBus bus;

	void runTest(string fileName, uint64_t expectedTStates);
	void loadFile(string fileName, uint16_t addr);

};
