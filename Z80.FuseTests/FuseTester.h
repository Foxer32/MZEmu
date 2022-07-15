#pragma once
#include <fstream>
#include <string>
#include <list>
#include <map>
#include <sstream>
#include <iostream>

#include "Z80.h"
#include "TestBus.h"

using namespace std;

class FuseTester
{
public:

	FuseTester();
	~FuseTester();

	void runTests();
	void printResults();

private:

	struct FuseTest
	{
		list<uint16_t> registers;
		list<uint8_t> states;
		list<list<uint16_t>> memory;
	};

	struct FuseExpected
	{
		list<list<string>> events;
		list<uint16_t> registers;
		list<uint8_t> states;
		list<list<uint16_t>> memory;
	};

	const string testFilePath = "tests/FUSE_Tests.txt";
	const string expectedFilePath = "tests/FUSE_Expected.txt";

	TestBus bus;

	map<string, FuseTest> tests;
	map<string, FuseExpected> expected;

	list<string> passing;
	map<string, list<string>> failing;

	void initRegisters(list<uint16_t> registers);
	void initMemory(list<list<uint16_t>> memory);
	void initStates(list<uint8_t> states);

	void readFuseTestsFile();
	void readFuseExpectedFile();

	bool compareActualWithExpected(uint8_t totalTStates,const FuseExpected& expected, list<string>& failDetails);

	list<string> readFile(const string& filename);
	list<string> splitString(const string& str, char separator = ' ');

	string boolToString(bool v);

	template <typename T>
	list<T> convertStringsToHex(const list<string>& strings);

	template <typename T>
	T getListItem(const list<T>& lst, int iterator);

	template<typename T>
	string toStringX(T v);

	template<typename T>
	string toStringB(T v);

};
