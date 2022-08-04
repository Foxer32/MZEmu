#pragma once
#include <cstdint>
#include <string>

class WavPlayer
{
public:
    WavPlayer();
    ~WavPlayer();

    void setSampleFrequency(uint32_t sampleRate);
    float updateAudio();
    void readFile(std::string path);

    void play();
    void pause();
    void stop();

private:
    void convertDataToSamples(uint8_t* data);

    struct WavHeader
    {
        uint8_t         chunkID[4];
        uint32_t        chunkSize;
        uint8_t         format[4];
        uint8_t         subchunk1ID[4];
        uint32_t        subchunk1Size;
        uint16_t        audioFormat;
        uint16_t        numChannels;
        uint32_t        sampleRate;
        uint32_t        byteRate;
        uint16_t        blockAlign;
        uint16_t        bitsPerSample;
        uint8_t         subchunk2ID[4];
        uint32_t        subchunk2Size;
    };

    WavHeader header;
    uint32_t updateSampleRate = 0;
    uint32_t sampleIterator = 0;
    uint32_t sampleCount = 0;
    float* samples = nullptr;
    bool canPlay = false;
    bool isPlaying = false;
};
