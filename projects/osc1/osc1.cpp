#include "daisy_seed.h"
#include "daisysp.h"

using namespace daisy;
using namespace daisysp;

static DaisySeed seed;
static Oscillator osc;

static void AudioCallback(float *in, float *out, size_t size)
{
    float output;

    // size 分だけ output を埋める
    for (size_t i = 0; i < size; i += 2)
    {
        output = osc.Process();

        // left out
        out[i] = output;

        // right out
        out[i + 1] = output;
    }
}

int main(void)
{
    // ハードウェアを初期化
    seed.Configure();
    seed.Init();

    // オシレーターを初期化
    float samplerate = seed.AudioSampleRate();
    osc.Init(samplerate);

    // 適当な波形、音量、周波数を設定
    osc.SetWaveform(osc.WAVE_SIN);
    osc.SetAmp(1.f);
    osc.SetFreq(1000);

    // 音を出す
    seed.StartAudio(AudioCallback);

    // 無限ループ
    for (;;)
    {
    }
}