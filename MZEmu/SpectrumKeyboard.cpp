#include "SpectrumKeyboard.h"
#include "WinNativeKeycodes.h"

SpectrumKeyboard::SpectrumKeyboard()
{
	for (auto& i : keyMatrix) i = 0xFF;

	keyMap =
	{
		{WinKey::Key_Shift, new uint8_t[2]{0, 1}},
		{WinKey::Key_Z, new uint8_t[2]{0, 2}},
		{WinKey::Key_X, new uint8_t[2]{0, 4}},
		{WinKey::Key_C, new uint8_t[2]{0, 8}},
		{WinKey::Key_V, new uint8_t[2]{0, 16}},
		{WinKey::Key_A, new uint8_t[2]{1, 1}},
		{WinKey::Key_S, new uint8_t[2]{1, 2}},
		{WinKey::Key_D, new uint8_t[2]{1, 4}},
		{WinKey::Key_F, new uint8_t[2]{1, 8}},
		{WinKey::Key_G, new uint8_t[2]{1, 16}},
		{WinKey::Key_Q, new uint8_t[2]{2, 1}},
		{WinKey::Key_W, new uint8_t[2]{2, 2}},
		{WinKey::Key_E, new uint8_t[2]{2, 4}},
		{WinKey::Key_R, new uint8_t[2]{2, 8}},
		{WinKey::Key_T, new uint8_t[2]{2, 16}},
		{WinKey::Key_1, new uint8_t[2]{3, 1}},
		{WinKey::Key_2, new uint8_t[2]{3, 2}},
		{WinKey::Key_3, new uint8_t[2]{3, 4}},
		{WinKey::Key_4, new uint8_t[2]{3, 8}},
		{WinKey::Key_5, new uint8_t[2]{3, 16}},
		{WinKey::Key_0, new uint8_t[2]{4, 1}},
		{WinKey::Key_9, new uint8_t[2]{4, 2}},
		{WinKey::Key_8, new uint8_t[2]{4, 4}},
		{WinKey::Key_7, new uint8_t[2]{4, 8}},
		{WinKey::Key_6, new uint8_t[2]{4, 16}},
		{WinKey::Key_P, new uint8_t[2]{5, 1}},
		{WinKey::Key_O, new uint8_t[2]{5, 2}},
		{WinKey::Key_I, new uint8_t[2]{5, 4}},
		{WinKey::Key_U, new uint8_t[2]{5, 8}},
		{WinKey::Key_Y, new uint8_t[2]{5, 16}},
		{WinKey::Key_Return, new uint8_t[2]{6, 1}},
		{WinKey::Key_L, new uint8_t[2]{6, 2}},
		{WinKey::Key_K, new uint8_t[2]{6, 4}},
		{WinKey::Key_J, new uint8_t[2]{6, 8}},
		{WinKey::Key_H, new uint8_t[2]{6, 16}},
		{WinKey::Key_Space, new uint8_t[2]{7, 1}},
		{WinKey::Key_Control, new uint8_t[2]{7, 2}},
		{WinKey::Key_M, new uint8_t[2]{7, 4}},
		{WinKey::Key_N, new uint8_t[2]{7, 8}},
		{WinKey::Key_B, new uint8_t[2]{7, 16}},

		{WinKey::Key_Back, new uint8_t[5]{0x28, 0, 1, 4, 1}},
		{WinKey::Key_Left, new uint8_t[5]{0x28, 0, 1, 3, 16}},
		{WinKey::Key_Right, new uint8_t[5]{0x28, 0, 1, 4, 4}},
		{WinKey::Key_Up, new uint8_t[5]{0x28, 0, 1, 4, 8}},
		{WinKey::Key_Down, new uint8_t[5]{0x28, 0, 1, 4, 16}}
	};
}

SpectrumKeyboard::~SpectrumKeyboard()
{
	for (auto& kv : keyMap)
		delete[] kv.second;
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

void SpectrumKeyboard::keyPressed(int key)
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

void SpectrumKeyboard::keyReleased(int key)
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
