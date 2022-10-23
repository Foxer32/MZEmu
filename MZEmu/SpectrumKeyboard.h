#pragma once
#include <cstdint>
#include <map>

class SpectrumKeyboard
{
public:
	SpectrumKeyboard();
	~SpectrumKeyboard();

	uint8_t getKey(uint8_t keyRow);

	void keyPressed(int key);
	void keyReleased(int key);

private:
	std::map <int, uint8_t*> keyMap;
	uint8_t keyMatrix[8];

	uint8_t row = 0;
	uint8_t value = 0;
};
