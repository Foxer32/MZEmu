#include <iostream>
#include <string> 
#include <cstdint>
#include "Specrtum128kBus.h"

#define OLC_PGE_APPLICATION
#include "olcPixelGameEngine.h"
#define OLC_PGEX_SOUND
#include "olcPGEX_Sound.h"

Specrtum128kBus bus;

class TestEmulationPGE : public olc::PixelGameEngine
{
public:

    TestEmulationPGE()
    {
        sAppName = "Test";
    }

private:

	bool hiSpeed = false;

private:

    static float soundOut(int nChannel, float fGlobalTime, float fSample)
    {
		bus.audioIn = fSample;
		bus.clock();
        return bus.audioOut;
    }

    bool OnUserCreate() override
    {
        bus.reset();
        bus.keyboard.connectApp(this);

        bus.cpu.cpuFrequency = 3500000;
        bus.setSampleFrequency(44100);
        olc::SOUND::InitialiseAudio(44100, 1, 8, 512);
        olc::SOUND::SetUserFilterFunction(soundOut);
		
        return true;
    }

    bool OnUserDestroy() override
    {
        olc::SOUND::DestroyAudio();
        return true;
    }

    bool OnUserUpdate(float elapsedTime) override
    {
        if (bus.video.screenReady)
        {
            bus.video.screenReady = false;
            DrawSprite(0, 0, &bus.video.screenBuffer);
        }

		bus.keyboard.updateMachineInput();
		updateAppInput();

        return true;
    }

	void updateAppInput()
	{
		if (GetKey(olc::Key::F1).bPressed)
			bus.reset();

		if (GetKey(olc::Key::F2).bPressed)
			olc::SOUND::PlaySample(olc::SOUND::LoadAudioSample("tests/z80full.wav"));

		if (GetKey(olc::Key::F3).bPressed)
		{
			hiSpeed = !hiSpeed;
			sAppName = hiSpeed ? "Test Hi Speed" : "Test";
			bus.setSampleFrequency(hiSpeed ? 4410 : 44100);
		}
	}

};

int main()
{
    TestEmulationPGE demo;
	if (demo.Construct(320, 256, 3, 3, false, true))
        demo.Start();

    return 0;
}
