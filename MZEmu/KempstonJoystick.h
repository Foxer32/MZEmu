#pragma once
#include <cstdint>
#include <map>

class KempstonJoystick
{
public:
	KempstonJoystick();
	~KempstonJoystick();

	uint8_t getJoystickPort();

	void keyPressed(int key);
	void keyReleased(int key);

private:
	std::map <int, uint8_t> keyMap;
	uint8_t joystickPort = 0;
};
