#include "SpectrumBus.h"

olcNoiseMaker<short>* SpectrumBus::noiseMaker = nullptr;

SpectrumBus::SpectrumBus()
{
	//wavPlayer.deleteAfterStop = true;
	cpu.cpuFrequency = 3500000;

	cpu.connectBus(this);
	video.connectBus(this);
}

SpectrumBus::~SpectrumBus()
{

}

void SpectrumBus::stopSound()
{
	noiseMaker->Stop();
	//delete noiseMaker;
}

void SpectrumBus::setSampleFrequency(uint32_t sampleRate)
{
	std::vector<std::wstring> devices = olcNoiseMaker<short>::Enumerate();
	noiseMaker = new olcNoiseMaker<short>(devices[0], 44100, 1, 8, 256);
	noiseMaker->SetUserFunction([&](int nChanel, float dTime) -> float { return makeNoise(nChanel, dTime); });

	cpu.setSampleFrequency(sampleRate);
	video.setSampleFrequency(sampleRate);
	wavPlayer.setSampleFrequency(sampleRate);
}

void SpectrumBus::setPausedStatus(bool status)
{
	pausedStatus = status;

	if (status)
		wavPlayer.pause();
	else
		wavPlayer.resume();
}

void SpectrumBus::setMaxSpeedStatus(bool status)
{
	maxSpeedStatus = status;
}

float SpectrumBus::makeNoise(int nChanel, float dTime)
{
	if (!pausedStatus)
	{
		do
		{
			audioIn = wavPlayer.updateAudio();
			clock();
		} while (maxSpeedStatus && !pausedStatus);
	}

	return audioOut;
}
