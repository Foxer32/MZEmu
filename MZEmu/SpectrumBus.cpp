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

	std::vector<std::wstring> devices = olcNoiseMaker<int16_t>::Enumerate();
	noiseMaker = new olcNoiseMaker<int16_t>(devices[0], sampleRate, 2, 8, 512);
	noiseMaker->SetUserFunction([&](int nChanel) -> int16_t { return makeNoise(nChanel); });

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
	video.fastRender = maxSpeedStatus;
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
	return (execTime / (1.0f / getSampleFrequency() * 1000000.0f)) * 100.0f;
}

int16_t SpectrumBus::makeNoise(int nChanel)
{
	if (nChanel == 0)
	{
		tStart = high_resolution_clock::now();

		if (!pausedStatus)
		{
			do
			{
				audioIn = wavPlayer.updateAudio();
				clock();
			} while (maxSpeedStatus && !pausedStatus);
		}

		tEnd = high_resolution_clock::now();

		execTime = duration<float, std::micro>(tEnd - tStart).count();

		//removeDC();
	}

	return audioOut[nChanel];
}

void SpectrumBus::removeDC()
{
	bool corectChannnel0 = audioOut[0] == lastOutChannnel0;
	bool corectChannnel1 = audioOut[1] == lastOutChannnel1;

	lastOutChannnel0 = audioOut[0];
	lastOutChannnel1 = audioOut[1];

	if (corectChannnel0)
	{
		if (correctionChannnel0 < divisor)
			correctionChannnel0++;
	}
	else
	{
		correctionChannnel0 = 1;
	}

	if (corectChannnel1)
	{
		if (correctionChannnel1 < divisor)
			correctionChannnel1++;
	}
	else
	{
		correctionChannnel1 = 1;
	}

	audioOut[0] = (audioOut[0] * (divisor - (dividend * correctionChannnel0)) / divisor);
	audioOut[1] = (audioOut[1] * (divisor - (dividend * correctionChannnel1)) / divisor);
}
