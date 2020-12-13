#include "daisy_seed.h"
#include "daisysp.h"

using namespace daisy;
using namespace daisysp;

static DaisySeed seed;
static Oscillator osc;

AdEnv env;

Switch button1;

static void AudioCallback(float *in, float *out, size_t size)
{
    float output, env_out;

    button1.Debounce();

    // 
    if (button1.RisingEdge())
    {
        env.Trigger();
    }

    // size 分だけ output を埋める
    for (size_t i = 0; i < size; i += 2)
    {
        env_out = env.Process();
        osc.SetAmp(env_out);

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

    // ボタンを初期化
    button1.Init(seed.GetPin(15), samplerate / 48.f);

    // 適当な波形、音量、周波数を設定
    osc.SetWaveform(osc.WAVE_SIN);
    osc.SetAmp(1.f);
    osc.SetFreq(220);

    // エンベロープを初期化
    env.Init(samplerate);

    // attackとdecayを設定
    env.SetTime(ADENV_SEG_ATTACK, .01);
    env.SetTime(ADENV_SEG_DECAY, .4);

    // エンベロープの範囲を設定
    env.SetMin(0.0);
    env.SetMax(1.f);

    // エンベロープのカーブを指定（0は線形）
    env.SetCurve(0);

    // 音を出す
    seed.StartAudio(AudioCallback);

    // 無限ループ
    for (;;)
    {
    }
}