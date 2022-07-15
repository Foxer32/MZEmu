#include "FuseTester.h"

FuseTester::FuseTester()
{

}

FuseTester::~FuseTester()
{

}

void FuseTester::runTests()
{
	readFuseTestsFile();
	readFuseExpectedFile();

    for (auto& test : tests)
    {
        string testName = test.first;

        bus.cpu.reset(true);

        FuseTest testToRun = test.second;

        initRegisters(testToRun.registers);
        initMemory(testToRun.memory);
        initStates(testToRun.states);

        uint16_t runToAddress = getListItem<uint16_t>(expected[testName].registers, 11);
        uint8_t totalTStates = 0;

        do
        {
            totalTStates += bus.cpu.step();
        } while (bus.cpu.PC < runToAddress);

        list<string> failDetails;
        bool pass = compareActualWithExpected(totalTStates, expected[testName], failDetails);

        if (pass)
            passing.push_back(testName);
        else
            failing.insert(make_pair(testName, failDetails));
    }
}

void FuseTester::initRegisters(list<uint16_t> registers)
{
    bus.cpu.A = getListItem<uint16_t>(registers, 0) >> 8;
    bus.cpu.F = getListItem<uint16_t>(registers, 0) & 0xFF;
    bus.cpu.B = getListItem<uint16_t>(registers, 1) >> 8;
    bus.cpu.C = getListItem<uint16_t>(registers, 1) & 0xFF;
    bus.cpu.D = getListItem<uint16_t>(registers, 2) >> 8;
    bus.cpu.E = getListItem<uint16_t>(registers, 2) & 0xFF;
    bus.cpu.H = getListItem<uint16_t>(registers, 3) >> 8;
    bus.cpu.L = getListItem<uint16_t>(registers, 3) & 0xFF;

    bus.cpu.A1 = getListItem<uint16_t>(registers, 4) >> 8;
    bus.cpu.F1 = getListItem<uint16_t>(registers, 4) & 0xFF;
    bus.cpu.B1 = getListItem<uint16_t>(registers, 5) >> 8;
    bus.cpu.C1 = getListItem<uint16_t>(registers, 5) & 0xFF;
    bus.cpu.D1 = getListItem<uint16_t>(registers, 6) >> 8;
    bus.cpu.E1 = getListItem<uint16_t>(registers, 6) & 0xFF;
    bus.cpu.H1 = getListItem<uint16_t>(registers, 7) >> 8;
    bus.cpu.L1 = getListItem<uint16_t>(registers, 7) & 0xFF;

    bus.cpu.IX = getListItem<uint16_t>(registers, 8);
    bus.cpu.IY = getListItem<uint16_t>(registers, 9);
    bus.cpu.SP = getListItem<uint16_t>(registers, 10);
    bus.cpu.PC = getListItem<uint16_t>(registers, 11);
    bus.cpu.MEMPTR = getListItem<uint16_t>(registers, 12);
}

void FuseTester::initMemory(list<list<uint16_t>> memory)
{
    for (auto& i : bus.mem) i = 0x00;

    for (auto& memBlock : memory)
    {
        uint16_t address = getListItem<uint16_t>(memBlock, 0);
        
        int i = 0;
        for (auto& item : memBlock)
        {
            if (i != 0)
            {
                bus.writeMemory(address, item);
                address++;
            }

            i++;
        }
    }
}

void FuseTester::initStates(list<uint8_t> states)
{
    bus.cpu.I = getListItem<uint8_t>(states, 0);
    bus.cpu.R = getListItem<uint8_t>(states, 1);
    bus.cpu.IFF1 = getListItem<uint8_t>(states, 2);
    bus.cpu.IFF2 = getListItem<uint8_t>(states, 3);
}

void FuseTester::readFuseTestsFile()
{
    list<string> lines = readFile(testFilePath);
    FuseTest fuseTest;
    bool newTest = true;
    uint8_t lineType = 1;
    string testName;

    for (auto& line : lines)
    {

        if (line.length() == 0)
            continue;

        if (newTest)
        {
            fuseTest = FuseTest();
            newTest = false;
        }

        if (line.rfind("-1", 0) == 0)
        {
            tests.insert(make_pair(testName, fuseTest));
            newTest = true;
            lineType = 1;
            continue;
        }

        if (lineType == 1)
        {
            testName = line;
            lineType++;
            continue;
        }

        if (lineType == 2)
        {
            list<string> regs = splitString(line);
            regs.remove_if([](string str) { return str.empty(); });

            fuseTest.registers = convertStringsToHex<uint16_t>(regs);
            lineType++;
            continue;
        }

        if (lineType == 3)
        {
            list<string> states = splitString(line);
            states.remove_if([](string str) { return str.empty(); });

            fuseTest.states = convertStringsToHex<uint8_t>(states);
            lineType++;
            continue;
        }

        if (lineType == 4)
        {
            list<string> mem = splitString(line);
            mem.remove_if([](string str) { return str.empty(); });
            mem.remove_if([](string str) { return str.compare("-1") == 0; });

            fuseTest.memory.push_back(convertStringsToHex<uint16_t>(mem));
            continue;
        }
    }
}

void FuseTester::readFuseExpectedFile()
{
    list<string> lines = readFile(expectedFilePath);
    FuseExpected fuseExpected;
    bool newExpected = true;
    uint8_t lineType = 1;
    string testName;

    for (auto& line : lines)
    {
        if (line.length() == 0)
        {
            expected.insert(make_pair(testName, fuseExpected));
            newExpected = true;
            lineType = 1;
            continue;
        }

        if (newExpected)
        {
            fuseExpected = FuseExpected();
            newExpected = false;
        }

        if (lineType == 1)
        {
            testName = line;
            lineType++;
            continue;
        }

        if (lineType == 2)
        {
            if (line.rfind(" ", 0) == 0)
            {
                list<string> anEvent = splitString(line);
                anEvent.remove_if([](string str) { return str.empty(); });

                fuseExpected.events.push_back(anEvent);
                continue;
            }
            lineType++;
        }

        if (lineType == 3)
        {
            list<string> regs = splitString(line);
            regs.remove_if([](string str) { return str.empty(); });

            fuseExpected.registers = convertStringsToHex<uint16_t>(regs);
            lineType++;
            continue;
        }

        if (lineType == 4)
        {
            list<string> states = splitString(line);
            states.remove_if([](string str) { return str.empty(); });

            list<uint8_t> statesHex = convertStringsToHex<uint8_t>(states);
            statesHex.pop_back();
            statesHex.push_back(atoi(states.back().c_str()));
            fuseExpected.states = statesHex;
            lineType++;
            continue;
        }

        if (lineType == 5)
        {
            list<string> mem = splitString(line);
            mem.remove_if([](string str) { return str.empty(); });
            mem.remove_if([](string str) { return str.compare("-1") == 0; });

            fuseExpected.memory.push_back(convertStringsToHex<uint16_t>(mem));
            continue;
        }
    }
}

bool FuseTester::compareActualWithExpected(uint8_t totalTStates, const FuseExpected& expected, list<string>& failDetails)
{
    bool pass = true;
    failDetails.clear();

    if (bus.cpu.A != (getListItem<uint16_t>(expected.registers, 0) >> 8))
    {
        failDetails.push_back("A expected " + toStringX((uint8_t)(getListItem<uint16_t>(expected.registers, 0) >> 8)) + " got " + toStringX(bus.cpu.A));
        pass = false;
    }

    if (bus.cpu.F != (getListItem<uint16_t>(expected.registers, 0) & 0xFF))
    {
        failDetails.push_back("               SZUHXPNC     SZUHXPNC");
        failDetails.push_back("Flags expected " + toStringB((uint8_t)(getListItem<uint16_t>(expected.registers, 0) & 0xFF)) + " got " + toStringB(bus.cpu.F));
        pass = false;
    }

    if (bus.cpu.B != (getListItem<uint16_t>(expected.registers, 1) >> 8))
    {
        failDetails.push_back("B expected " + toStringX((uint8_t)(getListItem<uint16_t>(expected.registers, 1) & 0xFF)) + " got " + toStringX(bus.cpu.B));
        pass = false;
    }

    if (bus.cpu.C != (getListItem<uint16_t>(expected.registers, 1) & 0xFF))
    {
        failDetails.push_back("C expected " + toStringX((uint8_t)(getListItem<uint16_t>(expected.registers, 1) & 0xFF)) + " got " + toStringX(bus.cpu.C));
        pass = false;
    }

    if (bus.cpu.D != (getListItem<uint16_t>(expected.registers, 2) >> 8))
    {
        failDetails.push_back("D expected " + toStringX((uint8_t)(getListItem<uint16_t>(expected.registers, 2) >> 8)) + " got " + toStringX(bus.cpu.D));
        pass = false;
    }

    if (bus.cpu.E != (getListItem<uint16_t>(expected.registers, 2) & 0xFF))
    {
        failDetails.push_back("E expected " + toStringX((uint8_t)(getListItem<uint16_t>(expected.registers, 2) & 0xFF)) + " got " + toStringX(bus.cpu.E));
        pass = false;
    }

    if (bus.cpu.H != (getListItem<uint16_t>(expected.registers, 3) >> 8))
    {
        failDetails.push_back("H expected " + toStringX((uint8_t)(getListItem<uint16_t>(expected.registers, 3) >> 8)) + " got " + toStringX(bus.cpu.H));
        pass = false;
    }

    if (bus.cpu.L != (getListItem<uint16_t>(expected.registers, 3) & 0xFF))
    {
        failDetails.push_back("L expected " + toStringX((uint8_t)(getListItem<uint16_t>(expected.registers, 3) & 0xFF)) + " got " + toStringX(bus.cpu.L));
        pass = false;
    }


    if (bus.cpu.A1 != (getListItem<uint16_t>(expected.registers, 4) >> 8))
    {
        failDetails.push_back("A' expected " + toStringX((uint8_t)(getListItem<uint16_t>(expected.registers, 4) >> 8)) + " got " + toStringX(bus.cpu.A1));
        pass = false;
    }

    if (bus.cpu.F1 != (getListItem<uint16_t>(expected.registers, 4) & 0xFF))
    {
        failDetails.push_back("                SZUHXPNC     SZUHXPNC");
        failDetails.push_back("Flags' expected " + toStringB((uint8_t)(getListItem<uint16_t>(expected.registers, 4) & 0xFF)) + " got " + toStringB(bus.cpu.F1));
        pass = false;
    }

    if (bus.cpu.B1 != (getListItem<uint16_t>(expected.registers, 5) >> 8))
    {
        failDetails.push_back("B' expected " + toStringX((uint8_t)(getListItem<uint16_t>(expected.registers, 5) & 0xFF)) + " got " + toStringX(bus.cpu.B1));
        pass = false;
    }

    if (bus.cpu.C1 != (getListItem<uint16_t>(expected.registers, 5) & 0xFF))
    {
        failDetails.push_back("C' expected " + toStringX((uint8_t)(getListItem<uint16_t>(expected.registers, 5) & 0xFF)) + " got " + toStringX(bus.cpu.C1));
        pass = false;
    }

    if (bus.cpu.D1 != (getListItem<uint16_t>(expected.registers, 6) >> 8))
    {
        failDetails.push_back("D' expected " + toStringX((uint8_t)(getListItem<uint16_t>(expected.registers, 6) >> 8)) + " got " + toStringX(bus.cpu.D1));
        pass = false;
    }

    if (bus.cpu.E1 != (getListItem<uint16_t>(expected.registers, 6) & 0xFF))
    {
        failDetails.push_back("E' expected " + toStringX((uint8_t)(getListItem<uint16_t>(expected.registers, 6) & 0xFF)) + " got " + toStringX(bus.cpu.E1));
        pass = false;
    }

    if (bus.cpu.H1 != (getListItem<uint16_t>(expected.registers, 7) >> 8))
    {
        failDetails.push_back("H' expected " + toStringX((uint8_t)(getListItem<uint16_t>(expected.registers, 7) >> 8)) + " got " + toStringX(bus.cpu.H1));
        pass = false;
    }

    if (bus.cpu.L1 != (getListItem<uint16_t>(expected.registers, 7) & 0xFF))
    {
        failDetails.push_back("L' expected " + toStringX((uint8_t)(getListItem<uint16_t>(expected.registers, 7) & 0xFF)) + " got " + toStringX(bus.cpu.L1));
        pass = false;
    }


    if (bus.cpu.IX != getListItem<uint16_t>(expected.registers, 8))
    {
        failDetails.push_back("IX expected " + toStringX(getListItem<uint16_t>(expected.registers, 8)) + " got " + toStringX(bus.cpu.IX));
        pass = false;
    }

    if (bus.cpu.IY != getListItem<uint16_t>(expected.registers, 9))
    {
        failDetails.push_back("IY expected " + toStringX(getListItem<uint16_t>(expected.registers, 9)) + " got " + toStringX(bus.cpu.IY));
        pass = false;
    }

    if (bus.cpu.SP != getListItem<uint16_t>(expected.registers, 10))
    {
        failDetails.push_back("SP expected " + toStringX(getListItem<uint16_t>(expected.registers, 10)) + " got " + toStringX(bus.cpu.SP));
        pass = false;
    }

    if (bus.cpu.PC != getListItem<uint16_t>(expected.registers, 11))
    {
        failDetails.push_back("PC expected " + toStringX(getListItem<uint16_t>(expected.registers, 11)) + " got " + toStringX(bus.cpu.PC));
        pass = false;
    }

    if (bus.cpu.MEMPTR != getListItem<uint16_t>(expected.registers, 12))
    {
        failDetails.push_back("MEMPTR expected " + toStringX(getListItem<uint16_t>(expected.registers, 12)) + " got " + toStringX(bus.cpu.MEMPTR));
        pass = false;
    }


    if (bus.cpu.I != (getListItem<uint8_t>(expected.states, 0)))
    {
        failDetails.push_back("I expected " + toStringX(getListItem<uint8_t>(expected.states, 0)) + " got " + toStringX(bus.cpu.I));
        pass = false;
    }

    if (bus.cpu.R != (getListItem<uint8_t>(expected.states, 1)))
    {
        failDetails.push_back("R expected " + toStringX(getListItem<uint8_t>(expected.states, 1)) + " got " + toStringX(bus.cpu.R));
        pass = false;
    }

    if (bus.cpu.IFF1 != (getListItem<uint8_t>(expected.states, 2)))
    {
        failDetails.push_back("IFF1 expected " + boolToString(getListItem<uint8_t>(expected.states, 2)) + " got " + boolToString(bus.cpu.IFF1));
        pass = false;
    }

    if (bus.cpu.IFF2 != (getListItem<uint8_t>(expected.states, 3)))
    {
        failDetails.push_back("IFF2 expected " + boolToString(getListItem<uint8_t>(expected.states, 3)) + " got " + boolToString(bus.cpu.IFF2));
        pass = false;
    }

    if (totalTStates != (getListItem<uint8_t>(expected.states, 6)))
    {
        failDetails.push_back("Timing expected " + to_string(getListItem<uint8_t>(expected.states, 6)) + " got " + to_string(totalTStates));
        pass = false;
    }


    for (auto& memBlock : expected.memory)
    {
        uint16_t address = getListItem<uint16_t>(memBlock, 0);

        int i = 0;
        for (auto& expectedByte : memBlock)
        {
            if (i != 0)
            {
                uint8_t actualByte = bus.readMemory(address);

                if (expectedByte != actualByte)
                {
                    failDetails.push_back("Address " + toStringX(address) + " expected " + toStringX((uint8_t)expectedByte) + " got " + toStringX(actualByte));
                    pass = false;
                }

                address++;
            }

            i++;
        }
    }

    return pass;
}

void FuseTester::printResults()
{
    cout << "Passed tests: " << to_string(passing.size()) << endl;
    cout << "Failed tests: " << to_string(failing.size()) << endl;

    if (failing.size() != 0)
    {
        for (const auto& [key, value] : failing)
        {
            cout << "\nTest name: " << key << endl;

            for (auto& i : value)
                cout << i << endl;

            system("pause");
        }
        cout << "\nIt was the last failed test" << endl;
    }
    else
    {
        cout << "\n!!! All tests passed !!!" << endl;
    }
    system("pause");
}

list<string> FuseTester::readFile(const string& filename)
{
    list<string> lines;
    ifstream file(filename);
    string s;

    while (getline(file, s))
        lines.push_back(s);

    return lines;
}

list<string> FuseTester::splitString(const string& str, char separator)
{
    list<string> lines;
    stringstream streamData(str);
    string buff;

    while (getline(streamData, buff, separator))
        lines.push_back(buff);

    return lines;
}

string FuseTester::boolToString(bool v)
{
    return v ? "true" : "false";
}

template <typename T>
list<T> FuseTester::convertStringsToHex(const list<string>& strings)
{
    list<T> buff;

    for (auto& i : strings)
        buff.push_back((T)stoul(i, nullptr, 16));

    return buff;
}

template <typename T>
T FuseTester::getListItem(const list<T>& lst, int iterator)
{
    int i = 0;
    for (auto& item : lst)
    {
        if (i == iterator)
            return item;
           
        i++;
    }
    return NULL;
}

template<typename T>
string FuseTester::toStringX(T v)
{
    uint8_t len = sizeof(T) * 2;

    std::string str(len, ' ');

    for (int16_t i = len - 1; i >= 0; i--, v >>= 4)
    {
        str[i] = "0123456789ABCDEF"[v & 0xF];
    }

    return str;
}

template<typename T>
string FuseTester::toStringB(T v)
{
    uint8_t len = sizeof(T) * 8;

    std::string str(len, ' ');

    for (int16_t i = len - 1; i >= 0; i--, v >>= 1)
    {
        str[i] = (v & 1) ? '1' : '0';
    }

    return str;
}
