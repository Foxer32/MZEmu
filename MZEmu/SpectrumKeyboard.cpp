#include "SpectrumKeyboard.h"

SpectrumKeyboard::SpectrumKeyboard()
{
	for (auto& i : keyMatrix) i = 0xFF;

	keyMap =
	{
		{Qt::Key_Shift, new uint8_t[2]{0, 1}},
		{Qt::Key_Z, new uint8_t[2]{0, 2}},
		{Qt::Key_X, new uint8_t[2]{0, 4}},
		{Qt::Key_C, new uint8_t[2]{0, 8}},
		{Qt::Key_V, new uint8_t[2]{0, 16}},
		{Qt::Key_A, new uint8_t[2]{1, 1}},
		{Qt::Key_S, new uint8_t[2]{1, 2}},
		{Qt::Key_D, new uint8_t[2]{1, 4}},
		{Qt::Key_F, new uint8_t[2]{1, 8}},
		{Qt::Key_G, new uint8_t[2]{1, 16}},
		{Qt::Key_Q, new uint8_t[2]{2, 1}},
		{Qt::Key_W, new uint8_t[2]{2, 2}},
		{Qt::Key_E, new uint8_t[2]{2, 4}},
		{Qt::Key_R, new uint8_t[2]{2, 8}},
		{Qt::Key_T, new uint8_t[2]{2, 16}},
		{Qt::Key_1, new uint8_t[2]{3, 1}},
		{Qt::Key_2, new uint8_t[2]{3, 2}},
		{Qt::Key_3, new uint8_t[2]{3, 4}},
		{Qt::Key_4, new uint8_t[2]{3, 8}},
		{Qt::Key_5, new uint8_t[2]{3, 16}},
		{Qt::Key_0, new uint8_t[2]{4, 1}},
		{Qt::Key_9, new uint8_t[2]{4, 2}},
		{Qt::Key_8, new uint8_t[2]{4, 4}},
		{Qt::Key_7, new uint8_t[2]{4, 8}},
		{Qt::Key_6, new uint8_t[2]{4, 16}},
		{Qt::Key_P, new uint8_t[2]{5, 1}},
		{Qt::Key_O, new uint8_t[2]{5, 2}},
		{Qt::Key_I, new uint8_t[2]{5, 4}},
		{Qt::Key_U, new uint8_t[2]{5, 8}},
		{Qt::Key_Y, new uint8_t[2]{5, 16}},
		{Qt::Key_Return, new uint8_t[2]{6, 1}},
		{Qt::Key_L, new uint8_t[2]{6, 2}},
		{Qt::Key_K, new uint8_t[2]{6, 4}},
		{Qt::Key_J, new uint8_t[2]{6, 8}},
		{Qt::Key_H, new uint8_t[2]{6, 16}},
		{Qt::Key_Space, new uint8_t[2]{7, 1}},
		{Qt::Key_Control, new uint8_t[2]{7, 2}},
		{Qt::Key_M, new uint8_t[2]{7, 4}},
		{Qt::Key_N, new uint8_t[2]{7, 8}},
		{Qt::Key_B, new uint8_t[2]{7, 16}},

		{Qt::Key_Backspace, new uint8_t[5]{0x28, 0, 1, 4, 1}},
		{Qt::Key_Left, new uint8_t[5]{0x28, 0, 1, 3, 16}},
		{Qt::Key_Right, new uint8_t[5]{0x28, 0, 1, 4, 4}},
		{Qt::Key_Up, new uint8_t[5]{0x28, 0, 1, 4, 8}},
		{Qt::Key_Down, new uint8_t[5]{0x28, 0, 1, 4, 16}}
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
