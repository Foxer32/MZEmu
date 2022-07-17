#include "SpectrumKeyboard.h"

SpectrumKeyboard::SpectrumKeyboard()
{
	for (auto& i : keyMatrix) i = 0xFF;

	keyMap =
	{
		{olc::Key::SHIFT, new uint8_t[2]{0, 1}},
		{olc::Key::Z, new uint8_t[2]{0, 2}},
		{olc::Key::X, new uint8_t[2]{0, 4}},
		{olc::Key::C, new uint8_t[2]{0, 8}},
		{olc::Key::V, new uint8_t[2]{0, 16}},
		{olc::Key::A, new uint8_t[2]{1, 1}},
		{olc::Key::S, new uint8_t[2]{1, 2}},
		{olc::Key::D, new uint8_t[2]{1, 4}},
		{olc::Key::F, new uint8_t[2]{1, 8}},
		{olc::Key::G, new uint8_t[2]{1, 16}},
		{olc::Key::Q, new uint8_t[2]{2, 1}},
		{olc::Key::W, new uint8_t[2]{2, 2}},
		{olc::Key::E, new uint8_t[2]{2, 4}},
		{olc::Key::R, new uint8_t[2]{2, 8}},
		{olc::Key::T, new uint8_t[2]{2, 16}},
		{olc::Key::K1, new uint8_t[2]{3, 1}},
		{olc::Key::K2, new uint8_t[2]{3, 2}},
		{olc::Key::K3, new uint8_t[2]{3, 4}},
		{olc::Key::K4, new uint8_t[2]{3, 8}},
		{olc::Key::K5, new uint8_t[2]{3, 16}},
		{olc::Key::K0, new uint8_t[2]{4, 1}},
		{olc::Key::K9, new uint8_t[2]{4, 2}},
		{olc::Key::K8, new uint8_t[2]{4, 4}},
		{olc::Key::K7, new uint8_t[2]{4, 8}},
		{olc::Key::K6, new uint8_t[2]{4, 16}},
		{olc::Key::P, new uint8_t[2]{5, 1}},
		{olc::Key::O, new uint8_t[2]{5, 2}},
		{olc::Key::I, new uint8_t[2]{5, 4}},
		{olc::Key::U, new uint8_t[2]{5, 8}},
		{olc::Key::Y, new uint8_t[2]{5, 16}},
		{olc::Key::ENTER, new uint8_t[2]{6, 1}},
		{olc::Key::L, new uint8_t[2]{6, 2}},
		{olc::Key::K, new uint8_t[2]{6, 4}},
		{olc::Key::J, new uint8_t[2]{6, 8}},
		{olc::Key::H, new uint8_t[2]{6, 16}},
		{olc::Key::SPACE, new uint8_t[2]{7, 1}},
		{olc::Key::CTRL, new uint8_t[2]{7, 2}},
		{olc::Key::M, new uint8_t[2]{7, 4}},
		{olc::Key::N, new uint8_t[2]{7, 8}},
		{olc::Key::B, new uint8_t[2]{7, 16}},

		{olc::Key::BACK, new uint8_t[5]{0x28, 0, 1, 4, 1}},
		{olc::Key::LEFT, new uint8_t[5]{0x28, 0, 1, 3, 16}},
		{olc::Key::RIGHT, new uint8_t[5]{0x28, 0, 1, 4, 4}},
		{olc::Key::UP, new uint8_t[5]{0x28, 0, 1, 4, 8}},
		{olc::Key::DOWN, new uint8_t[5]{0x28, 0, 1, 4, 16}}
	};
}

SpectrumKeyboard::~SpectrumKeyboard()
{
	for (auto& [key, value] : keyMap)
		delete[] value;
}

uint8_t SpectrumKeyboard::getKey(uint8_t keyRow)
{
	switch (keyRow)
	{
	case 0x7F:
		return keyMatrix[7];
	case 0xBF:
		return keyMatrix[6];
	case 0xDF:
		return keyMatrix[5];
	case 0xEF:
		return keyMatrix[4];
	case 0xF7:
		return keyMatrix[3];
	case 0xFB:
		return keyMatrix[2];
	case 0xFD:
		return keyMatrix[1];
	case 0xFE:
		return keyMatrix[0];
	default:
		return 0xFF;
	}
}

void SpectrumKeyboard::keyPressed(olc::Key key)
{
	if (keyMap.find(key) != keyMap.end())
	{
		row = (keyMap[key])[0];

		if (row & 0xF8) // Key combinations
		{
			uint8_t tempRow = (row >> 4) << 1;
			for (uint8_t i = 0; i < tempRow; i += 2)
			{
				row = (keyMap[key])[i + 1];
				value = (keyMap[key])[i + 2];
				keyMatrix[row] &= ~value;
			}
		}
		else
		{
			value = (keyMap[key])[1];
			keyMatrix[row] &= ~value;
		}
	}
}

void SpectrumKeyboard::keyReleased(olc::Key key)
{
	if (keyMap.find(key) != keyMap.end())
	{
		row = (keyMap[key])[0];

		if (row & 0xF8) // Key combinations
		{
			uint8_t tempRow = (row >> 4) << 1;
			for (uint8_t i = 0; i < tempRow; i += 2)
			{
				row = (keyMap[key])[i + 1];
				value = (keyMap[key])[i + 2];
				keyMatrix[row] |= value;
			}
		}
		else
		{
			value = (keyMap[key])[1];
			keyMatrix[row] |= value;
		}
	}
}

void SpectrumKeyboard::updateMachineInput()
{	
	for (uint16_t i = 0; i < 256; i++)
	{
		key = static_cast<olc::Key>(i);

		if (app->GetKey(key).bPressed)
			keyPressed(key);

		if (app->GetKey(key).bReleased)
			keyReleased(key);
	}
}
