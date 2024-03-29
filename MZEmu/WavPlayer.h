#pragma once
#include <cstdint>
#include <functional>
#include <QApplication>

class WavPlayer : public QObject
{
    Q_OBJECT

signals:
    void updateProgressBar(int value);

public:
    WavPlayer();
    ~WavPlayer();

    void setSampleFrequency(uint32_t sampleRate);
    int16_t updateAudio();
    void readFile(std::string path);

    void play();
    void resume();
    void pause();
    void stop();

    std::string getOpenedFilePath();
    int getOpenedFileLength();

    bool deleteAfterStop = false;

private:
    void updatePercents();
    void convertDataToSamples(uint8_t* data);
    void deleteSamples();

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

    std::string openedFilePath = "";
    int openedFileLength = 0;

    WavHeader header;
    int oldPercents = 0;
    uint32_t updateSampleRate = 0;
    uint32_t sampleIterator = 0;
    uint32_t sampleCount = 0;
    int16_t* samples = nullptr;
    bool canPlay = false;
    bool isPlaying = false;
    bool isPaused = false;
};
