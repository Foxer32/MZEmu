#pragma once
#include <cstdint>

class IBus;

class Device
{
public:

	void connectBus(IBus* b) { bus = b; }

	IBus* bus = nullptr;
};
