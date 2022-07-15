#include <iostream>
#include <string> 
#include <cstdint>
#include "Specrtum48kBus.h"

#define OLC_PGE_APPLICATION
#include "olcPixelGameEngine.h"
#define OLC_PGEX_SOUND
#include "olcPGEX_Sound.h"

Specrtum48kBus bus;

class TestEmulationPGE : public olc::PixelGameEngine
{
public:

    TestEmulationPGE()
    {
        sAppName = "Test";
    }

private:

	int soundFile;
	bool hiSpeed = false;

private:

    static float soundOut(int nChannel, float fGlobalTime, float fSample)
    {
		bus.audioIn = fSample;
		bus.clock();
		return ((bus.audioOut * 0.5) + fSample) / 2;
    }

    bool OnUserCreate() override
    {
        bus.reset();

        bus.cpu.cpuFrequency = 3500000;
        bus.setSampleFrequency(44100);
        olc::SOUND::InitialiseAudio(44100, 1, 8, 512);
        olc::SOUND::SetUserFilterFunction(soundOut);
		
		soundFile = olc::SOUND::LoadAudioSample("tests/z80doc.wav");

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

		updateMachineInput();
		updateAppInput();

        return true;
    }

    void updateMachineInput()
    {
		if (GetKey(olc::Key::SHIFT).bHeld)
			bus.keyMatrix[0] = bus.keyMatrix[0] & ~(0x1);
		else
			bus.keyMatrix[0] = bus.keyMatrix[0] | (0x1);

		if (GetKey(olc::Key::Z).bPressed || GetKey(olc::Key::Z).bHeld)
			bus.keyMatrix[0] = bus.keyMatrix[0] & ~(0x02);
		else
			bus.keyMatrix[0] = bus.keyMatrix[0] | (0x02);

		if (GetKey(olc::Key::X).bPressed || GetKey(olc::Key::X).bHeld)
			bus.keyMatrix[0] = bus.keyMatrix[0] & ~(0x04);
		else
			bus.keyMatrix[0] = bus.keyMatrix[0] | (0x04);

		if (GetKey(olc::Key::C).bPressed || GetKey(olc::Key::C).bHeld)
			bus.keyMatrix[0] = bus.keyMatrix[0] & ~(0x08);
		else
			bus.keyMatrix[0] = bus.keyMatrix[0] | (0x08);

		if (GetKey(olc::Key::V).bPressed || GetKey(olc::Key::V).bHeld)
			bus.keyMatrix[0] = bus.keyMatrix[0] & ~(0x10);
		else
			bus.keyMatrix[0] = bus.keyMatrix[0] | (0x10);

		if (GetKey(olc::Key::A).bPressed || GetKey(olc::Key::A).bHeld)
			bus.keyMatrix[1] = bus.keyMatrix[1] & ~(0x1);
		else
			bus.keyMatrix[1] = bus.keyMatrix[1] | (0x1);

		if (GetKey(olc::Key::S).bPressed || GetKey(olc::Key::S).bHeld)
			bus.keyMatrix[1] = bus.keyMatrix[1] & ~(0x02);
		else
			bus.keyMatrix[1] = bus.keyMatrix[1] | (0x02);

		if (GetKey(olc::Key::D).bPressed || GetKey(olc::Key::D).bHeld)
			bus.keyMatrix[1] = bus.keyMatrix[1] & ~(0x04);
		else
			bus.keyMatrix[1] = bus.keyMatrix[1] | (0x04);

		if (GetKey(olc::Key::F).bPressed || GetKey(olc::Key::F).bHeld)
			bus.keyMatrix[1] = bus.keyMatrix[1] & ~(0x08);
		else
			bus.keyMatrix[1] = bus.keyMatrix[1] | (0x08);

		if (GetKey(olc::Key::G).bPressed || GetKey(olc::Key::G).bHeld)
			bus.keyMatrix[1] = bus.keyMatrix[1] & ~(0x10);
		else
			bus.keyMatrix[1] = bus.keyMatrix[1] | (0x10);

		if (GetKey(olc::Key::Q).bPressed || GetKey(olc::Key::Q).bHeld)
			bus.keyMatrix[2] = bus.keyMatrix[2] & ~(0x1);
		else
			bus.keyMatrix[2] = bus.keyMatrix[2] | (0x1);

		if (GetKey(olc::Key::W).bPressed || GetKey(olc::Key::W).bHeld)
			bus.keyMatrix[2] = bus.keyMatrix[2] & ~(0x02);
		else
			bus.keyMatrix[2] = bus.keyMatrix[2] | (0x02);

		if (GetKey(olc::Key::E).bPressed || GetKey(olc::Key::E).bHeld)
			bus.keyMatrix[2] = bus.keyMatrix[2] & ~(0x04);
		else
			bus.keyMatrix[2] = bus.keyMatrix[2] | (0x04);

		if (GetKey(olc::Key::R).bPressed || GetKey(olc::Key::R).bHeld)
			bus.keyMatrix[2] = bus.keyMatrix[2] & ~(0x08);
		else
			bus.keyMatrix[2] = bus.keyMatrix[2] | (0x08);

		if (GetKey(olc::Key::T).bPressed || GetKey(olc::Key::T).bHeld)
			bus.keyMatrix[2] = bus.keyMatrix[2] & ~(0x10);
		else
			bus.keyMatrix[2] = bus.keyMatrix[2] | (0x10);

		if (GetKey(olc::Key::K1).bPressed || GetKey(olc::Key::K1).bHeld)
			bus.keyMatrix[3] = bus.keyMatrix[3] & ~(0x1);
		else
			bus.keyMatrix[3] = bus.keyMatrix[3] | (0x1);

		if (GetKey(olc::Key::K2).bPressed || GetKey(olc::Key::K2).bHeld)
			bus.keyMatrix[3] = bus.keyMatrix[3] & ~(0x02);
		else
			bus.keyMatrix[3] = bus.keyMatrix[3] | (0x02);

		if (GetKey(olc::Key::K3).bPressed || GetKey(olc::Key::K3).bHeld)
			bus.keyMatrix[3] = bus.keyMatrix[3] & ~(0x04);
		else
			bus.keyMatrix[3] = bus.keyMatrix[3] | (0x04);

		if (GetKey(olc::Key::K4).bPressed || GetKey(olc::Key::K4).bHeld)
			bus.keyMatrix[3] = bus.keyMatrix[3] & ~(0x08);
		else
			bus.keyMatrix[3] = bus.keyMatrix[3] | (0x08);

		if (GetKey(olc::Key::K5).bPressed || GetKey(olc::Key::K5).bHeld)
			bus.keyMatrix[3] = bus.keyMatrix[3] & ~(0x10);
		else
			bus.keyMatrix[3] = bus.keyMatrix[3] | (0x10);

		if (GetKey(olc::Key::K0).bPressed || GetKey(olc::Key::K0).bHeld)
			bus.keyMatrix[4] = bus.keyMatrix[4] & ~(0x1);
		else
			bus.keyMatrix[4] = bus.keyMatrix[4] | (0x1);

		if (GetKey(olc::Key::K9).bPressed || GetKey(olc::Key::K9).bHeld)
			bus.keyMatrix[4] = bus.keyMatrix[4] & ~(0x02);
		else
			bus.keyMatrix[4] = bus.keyMatrix[4] | (0x02);

		if (GetKey(olc::Key::K8).bPressed || GetKey(olc::Key::K8).bHeld)
			bus.keyMatrix[4] = bus.keyMatrix[4] & ~(0x04);
		else
			bus.keyMatrix[4] = bus.keyMatrix[4] | (0x04);

		if (GetKey(olc::Key::K7).bPressed || GetKey(olc::Key::K7).bHeld)
			bus.keyMatrix[4] = bus.keyMatrix[4] & ~(0x08);
		else
			bus.keyMatrix[4] = bus.keyMatrix[4] | (0x08);

		if (GetKey(olc::Key::K6).bPressed || GetKey(olc::Key::K6).bHeld)
			bus.keyMatrix[4] = bus.keyMatrix[4] & ~(0x10);
		else
			bus.keyMatrix[4] = bus.keyMatrix[4] | (0x10);

		if (GetKey(olc::Key::P).bPressed || GetKey(olc::Key::P).bHeld)
			bus.keyMatrix[5] = bus.keyMatrix[5] & ~(0x1);
		else
			bus.keyMatrix[5] = bus.keyMatrix[5] | (0x1);

		if (GetKey(olc::Key::O).bPressed || GetKey(olc::Key::O).bHeld)
			bus.keyMatrix[5] = bus.keyMatrix[5] & ~(0x02);
		else
			bus.keyMatrix[5] = bus.keyMatrix[5] | (0x02);

		if (GetKey(olc::Key::I).bPressed || GetKey(olc::Key::I).bHeld)
			bus.keyMatrix[5] = bus.keyMatrix[5] & ~(0x04);
		else
			bus.keyMatrix[5] = bus.keyMatrix[5] | (0x04);

		if (GetKey(olc::Key::U).bPressed || GetKey(olc::Key::U).bHeld)
			bus.keyMatrix[5] = bus.keyMatrix[5] & ~(0x08);
		else
			bus.keyMatrix[5] = bus.keyMatrix[5] | (0x08);

		if (GetKey(olc::Key::Y).bPressed || GetKey(olc::Key::Y).bHeld)
			bus.keyMatrix[5] = bus.keyMatrix[5] & ~(0x10);
		else
			bus.keyMatrix[5] = bus.keyMatrix[5] | (0x10);

		if (GetKey(olc::Key::ENTER).bPressed || GetKey(olc::Key::ENTER).bHeld)
			bus.keyMatrix[6] = bus.keyMatrix[6] & ~(0x1);
		else
			bus.keyMatrix[6] = bus.keyMatrix[6] | (0x1);

		if (GetKey(olc::Key::L).bPressed || GetKey(olc::Key::L).bHeld)
			bus.keyMatrix[6] = bus.keyMatrix[6] & ~(0x02);
		else
			bus.keyMatrix[6] = bus.keyMatrix[6] | (0x02);

		if (GetKey(olc::Key::K).bPressed || GetKey(olc::Key::K).bHeld)
			bus.keyMatrix[6] = bus.keyMatrix[6] & ~(0x04);
		else
			bus.keyMatrix[6] = bus.keyMatrix[6] | (0x04);

		if (GetKey(olc::Key::J).bPressed || GetKey(olc::Key::J).bHeld)
			bus.keyMatrix[6] = bus.keyMatrix[6] & ~(0x08);
		else
			bus.keyMatrix[6] = bus.keyMatrix[6] | (0x08);

		if (GetKey(olc::Key::H).bPressed || GetKey(olc::Key::H).bHeld)
			bus.keyMatrix[6] = bus.keyMatrix[6] & ~(0x10);
		else
			bus.keyMatrix[6] = bus.keyMatrix[6] | (0x10);

		if (GetKey(olc::Key::SPACE).bPressed || GetKey(olc::Key::SPACE).bHeld)
			bus.keyMatrix[7] = bus.keyMatrix[7] & ~(0x1);
		else
			bus.keyMatrix[7] = bus.keyMatrix[7] | (0x1);


		if (GetKey(olc::Key::CTRL).bHeld)
			bus.keyMatrix[7] = bus.keyMatrix[7] & ~(0x02);
		else
			bus.keyMatrix[7] = bus.keyMatrix[7] | (0x02);

		if (GetKey(olc::Key::M).bPressed || GetKey(olc::Key::M).bHeld)
			bus.keyMatrix[7] = bus.keyMatrix[7] & ~(0x04);
		else
			bus.keyMatrix[7] = bus.keyMatrix[7] | (0x04);

		if (GetKey(olc::Key::N).bPressed || GetKey(olc::Key::N).bHeld)
			bus.keyMatrix[7] = bus.keyMatrix[7] & ~(0x08);
		else
			bus.keyMatrix[7] = bus.keyMatrix[7] | (0x08);

		if (GetKey(olc::Key::B).bPressed || GetKey(olc::Key::B).bHeld)
			bus.keyMatrix[7] = bus.keyMatrix[7] & ~(0x10);
		else
			bus.keyMatrix[7] = bus.keyMatrix[7] | (0x010);


		if (GetKey(olc::Key::BACK).bPressed || GetKey(olc::Key::DEL).bPressed ||
			GetKey(olc::Key::BACK).bHeld || GetKey(olc::Key::DEL).bHeld)
		{
			bus.keyMatrix[0] = bus.keyMatrix[0] & ~(0x1);
			bus.keyMatrix[4] = bus.keyMatrix[0] & ~(0x1);
		}

		if (GetKey(olc::Key::LEFT).bPressed || GetKey(olc::Key::LEFT).bHeld)
		{
			bus.keyMatrix[0] = bus.keyMatrix[0] & ~(0x1);
			bus.keyMatrix[3] = bus.keyMatrix[3] & ~(0x10);
		}

		if (GetKey(olc::Key::RIGHT).bPressed || GetKey(olc::Key::RIGHT).bHeld)
		{
			bus.keyMatrix[0] = bus.keyMatrix[0] & ~(0x1);
			bus.keyMatrix[4] = bus.keyMatrix[4] & ~(0x04);
		}

		if (GetKey(olc::Key::UP).bPressed || GetKey(olc::Key::UP).bHeld)
		{
			bus.keyMatrix[0] = bus.keyMatrix[0] & ~(0x1);
			bus.keyMatrix[4] = bus.keyMatrix[4] & ~(0x08);
		}

		if (GetKey(olc::Key::DOWN).bPressed || GetKey(olc::Key::DOWN).bHeld)
		{
			bus.keyMatrix[0] = bus.keyMatrix[0] & ~(0x1);
			bus.keyMatrix[4] = bus.keyMatrix[4] & ~(0x10);
		}
    }

	void updateAppInput()
	{
		if (GetKey(olc::Key::F1).bPressed)
			bus.reset();

		if (GetKey(olc::Key::F2).bPressed)
			olc::SOUND::PlaySample(soundFile);

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
