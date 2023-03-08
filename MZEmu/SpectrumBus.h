#pragma once

#include "IBus.h"
#include "olcNoiseMaker.h"
#include "SpectrumVideo.h"
#include "SpectrumKeyboard.h"
#include "KempstonJoystick.h"
#include "WavPlayer.h"

#include <chrono>
using std::chrono::steady_clock;
using std::chrono::high_resolution_clock;
using std::chrono::duration;

class SpectrumBus : public IBus
{
public:
	SpectrumBus();
	~SpectrumBus();

	virtual void reset(bool hardReset = false) = 0;
	virtual void clock() = 0;

	virtual void setSampleFrequency(uint32_t sampleRate);
	uint32_t getSampleFrequency();


	void stopSound();

	void setPausedStatus(bool status);
	void setMaxSpeedStatus(bool status);
	const char* getStatus();
	float getSystemLoad();

	SpectrumVideo video;
	SpectrumKeyboard keyboard;
	KempstonJoystick joystick;

	int16_t audioOut[2] = { 0,0 };
	int16_t audioIn = 0;

	WavPlayer wavPlayer;

protected:
	int16_t makeNoise(int nChanel);
	virtual void mixAudioInputs() = 0;

	bool pausedStatus = false;
	bool maxSpeedStatus = false;

	int16_t speakerOut = 0;

	static olcNoiseMaker<int16_t>* noiseMaker;

private:
	void removeDC();

	const int dividend = 1;
	const int divisor = 10000;

	int16_t lastOutChannnel0 = 0;
	int16_t lastOutChannnel1 = 0;

	int correctionChannnel0 = 0;
	int correctionChannnel1 = 0;

	steady_clock::time_point tStart;
	steady_clock::time_point tEnd;

	float execTime = 0;

	uint32_t sampleRate = 0;

};
