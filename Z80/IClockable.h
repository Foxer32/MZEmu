#pragma once

class IClockable
{
public:
	virtual int step() = 0;

	void setSampleFrequency(int sampleRate)
	{
		deviceUpdFreq = (float)deviceClockFrequency / (float)sampleRate;
	};

	void update()
	{
		tCount = lastTCount;
		while (tCount < deviceUpdFreq)
			tCount += step();
		lastTCount = tCount - deviceUpdFreq;
	};

protected:
	int deviceClockFrequency = 0;
	float deviceUpdFreq = 0;
	float lastTCount = 0;

private:
	float tCount = 0;

};
