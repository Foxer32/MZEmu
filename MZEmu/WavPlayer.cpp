#include "WavPlayer.h"
#include <fstream>
#include <iostream>
#include <exception>

WavPlayer::WavPlayer()
{

}

WavPlayer::~WavPlayer()
{
    if (samples != nullptr)
        delete[] samples;
}

void WavPlayer::setSampleFrequency(uint32_t sampleRate)
{
    updateSampleRate = sampleRate;
}

float WavPlayer::updateAudio()
{
    if (isPlaying)
    {
        if (sampleIterator < sampleCount)
            return samples[sampleIterator++];
        else
            stop();
    }

    return 0.0f;
}

void WavPlayer::readFile(std::string path)
{
    stop();

    std::ifstream ifs;
    ifs.open(path, std::ifstream::binary);
    if (ifs.is_open())
    {
        ifs.read((char*)&header, sizeof(struct WavHeader));

        if (strncmp((char*)header.chunkID, "RIFF", 4) != 0 ||
            strncmp((char*)header.format, "WAVE", 4) != 0 ||
            strncmp((char*)header.subchunk1ID, "fmt ", 4) != 0 ||
            strncmp((char*)header.subchunk2ID, "data", 4) != 0 ||
            header.subchunk1Size != 16 ||
            header.audioFormat != 1 ||
            header.sampleRate != updateSampleRate)
        {
            ifs.close();
            canPlay = false;
            throw std::exception("Wrong file");
        }

        uint8_t* data = new uint8_t[header.subchunk2Size];

        ifs.read((char*)data, (unsigned int)header.subchunk2Size);
        ifs.close();

        convertDataToSamples(data);
        delete[] data;

        canPlay = true;
    }
    else
    {
        throw std::exception("Can't open the file");
    }
}

void WavPlayer::play()
{
    isPlaying = canPlay;
}

void WavPlayer::pause()
{
    isPlaying = false;
}

void WavPlayer::stop()
{
    isPlaying = false;
    sampleIterator = 0;
}

void WavPlayer::convertDataToSamples(uint8_t* data)
{
    sampleCount = header.subchunk2Size / header.blockAlign;

    if (samples != nullptr)
        delete[] samples;

    samples = new float[sampleCount];

    bool isSigned = (header.bitsPerSample > 8);
    uint64_t bitMask = (_UI64_MAX >> (64 - header.bitsPerSample));
    for (uint32_t i = 0; i < sampleCount; i++)
    {
        samples[i] = 0;

        for (uint32_t j = 0; j < header.numChannels; j++)
        {
            int64_t buffSample2 = *(uint64_t*)(data + (i * header.blockAlign + j * (header.bitsPerSample >> 3))) & bitMask;

            if (isSigned && (buffSample2 >> (header.bitsPerSample - 1)))
                buffSample2 |= ~bitMask;

            samples[i] += (float)(buffSample2) / (float)(bitMask >> isSigned);
        }

        samples[i] /= header.numChannels;
    }
}