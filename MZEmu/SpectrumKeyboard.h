#pragma once
#include <cstdint>
#include <map>

#include "olcPixelGameEngine.h"

class SpectrumKeyboard
{
public:
	SpectrumKeyboard();
	~SpectrumKeyboard();

	uint8_t getKey(uint8_t keyRow);

	void keyPressed(olc::Key key);
	void keyReleased(olc::Key key);

	//It's temporarily
	olc::PixelGameEngine* app = nullptr;
	void connectApp(olc::PixelGameEngine* a) { app = a; }
	void updateMachineInput();

private:
	std::map <olc::Key, uint8_t*> keyMap;
	uint8_t keyMatrix[8];

	uint8_t row = 0;
	uint8_t value = 0;
	olc::Key key;
};
