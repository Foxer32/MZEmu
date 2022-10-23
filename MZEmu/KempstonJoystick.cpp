#include "KempstonJoystick.h"
#include "WinNativeKeycodes.h"

KempstonJoystick::KempstonJoystick()
{
	keyMap =
	{
		{WinKey::Key_Numpad6, 1},
		{WinKey::Key_Numpad4, 2},
		{WinKey::Key_Numpad5, 4},
		{WinKey::Key_Numpad8, 8},
		{WinKey::Key_Numpad0, 16}
	};
}

KempstonJoystick::~KempstonJoystick()
{

}

uint8_t KempstonJoystick::getJoystickPort()
{
	return joystickPort;
}

void KempstonJoystick::keyPressed(int key)
{
	if (keyMap.find(key) != keyMap.end())
		joystickPort |= keyMap[key];
}

void KempstonJoystick::keyReleased(int key)
{
	if (keyMap.find(key) != keyMap.end())
		joystickPort &= ~keyMap[key];
}
