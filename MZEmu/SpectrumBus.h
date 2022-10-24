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

	float audioOut[2] = { 0,0 };
	float audioIn = 0;

	WavPlayer wavPlayer;

protected:
	float makeNoise(int nChanel, float dTime);
	virtual void mixAudioInputs() = 0;

	bool pausedStatus = false;
	bool maxSpeedStatus = false;

	float speakerOut = 0;

	static olcNoiseMaker<short>* noiseMaker;

private:
	steady_clock::time_point tStart;
	steady_clock::time_point tEnd;

	float execTime = 0;

	uint32_t sampleRate = 0;

};
