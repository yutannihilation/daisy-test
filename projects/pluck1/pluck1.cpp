#include "daisy_seed.h"
#include "daisysp.h"

using namespace daisy;
using namespace daisysp;

#define BUFSIZE 256

static DaisySeed seed;
Pluck synth;
float plkbuff[BUFSIZE];

Switch button1;

static void AudioCallback(float *in, float *out, size_t size)
{
    float sig;
    float trig; // Pluck Vars

    button1.Debounce();

    if (button1.RisingEdge())
    {
        trig = 1.0f;
    } else {
        trig = 0.0f;
    }

    synth.SetDecay(seed.adc.GetFloat(0));

    for (size_t i = 0; i < size; i += 2)
    {
        synth.SetFreq(mtof(seed.adc.GetFloat(0) * 127));
        sig = synth.Process(trig);

        // left out
        out[i] = sig;

        // right out
        out[i + 1] = sig;
    }
}

int main(void)
{
    seed.Configure();
    seed.Init();

    float samplerate = seed.AudioSampleRate();

    // Use pin 16 to read the voltage of the knob
    AdcChannelConfig adcConfig;
    adcConfig.InitSingle(seed.GetPin(16));
    seed.adc.Init(&adcConfig, 1);

    synth.Init(samplerate, plkbuff, BUFSIZE, PLUCK_MODE_RECURSIVE);

    // Use pin 15 to read button to trigger the synth
    button1.Init(seed.GetPin(15), samplerate / 48.f);

    seed.adc.Start();
    seed.StartAudio(AudioCallback);

    for (;;)
    {
    }
}