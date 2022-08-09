#pragma once

#include "IBus.h"
#include "olcNoiseMaker.h"
#include "SpectrumVideo.h"
#include "SpectrumKeyboard.h"
#include "WavPlayer.h"

class SpectrumBus : public IBus
{
public:
	SpectrumBus();
	~SpectrumBus();

	void setSampleFrequency(uint32_t sampleRate);

	virtual void reset(bool hardReset = false) = 0;
	virtual void clock() = 0;

	void stopSound();

	void setPausedStatus(bool status);
	void setMaxSpeedStatus(bool status);

	SpectrumVideo video;
	SpectrumKeyboard keyboard;

	float audioOut = 0;
	float audioIn = 0;

	WavPlayer wavPlayer;

protected:
	float makeNoise(int nChanel, float dTime);
	virtual void mixAudioInputs() = 0;

	bool pausedStatus = false;
	bool maxSpeedStatus = false;

	float speakerOut = 0;

	static olcNoiseMaker<short>* noiseMaker;

};
