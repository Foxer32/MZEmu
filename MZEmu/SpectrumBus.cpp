#include "SpectrumBus.h"

olcNoiseMaker<short>* SpectrumBus::noiseMaker = nullptr;

SpectrumBus::SpectrumBus()
{
	//wavPlayer.deleteAfterStop = true;
	cpu.setFrequency(3500000);

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
	this->sampleRate = sampleRate;

	std::vector<std::wstring> devices = olcNoiseMaker<short>::Enumerate();
	noiseMaker = new olcNoiseMaker<short>(devices[0], sampleRate, 2, 8, 512);
	noiseMaker->SetUserFunction([&](int nChanel, float dTime) -> float { return makeNoise(nChanel, dTime); });

	cpu.setSampleFrequency(sampleRate);
	video.setSampleFrequency(sampleRate);
	wavPlayer.setSampleFrequency(sampleRate);
}

uint32_t SpectrumBus::getSampleFrequency()
{
	return sampleRate;
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

const char* SpectrumBus::getStatus()
{
	if (pausedStatus)
		return "Paused";

	if (maxSpeedStatus)
		return "Runnig at max speed";

	return "Runnig";
}

float SpectrumBus::getSystemLoad()
{
	return (execTime / (1.0f / 44100.0f * 1000000.0f)) * 100.0f;
}

float SpectrumBus::makeNoise(int nChanel, float dTime)
{
	if (!pausedStatus && (nChanel == 0))
	{
		tStart = high_resolution_clock::now();

		do
		{
			audioIn = wavPlayer.updateAudio();
			clock();
		} while (maxSpeedStatus && !pausedStatus);

		tEnd = high_resolution_clock::now();

		execTime = duration<float, std::micro>(tEnd - tStart).count();
	}

	return audioOut[nChanel];
}
