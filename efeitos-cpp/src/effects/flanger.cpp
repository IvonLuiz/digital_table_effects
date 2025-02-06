#define _USE_MATH_DEFINES

#include "effects/flanger.h"
#include <cmath>
#include <vector>
#include <algorithm>
#include <iostream>

struct FlangerState
{
    std::vector<float> delayBuffer;
    float phase;
    int writeIndex;
};

void flanger(std::vector<float> &samples, int sampleRate, float delayMs, float depthMs, float rateHz)
{

    FlangerState state;
    state.phase = 0.0f;
    float lfoFrequency = rateHz / sampleRate;

    int delayBufferSize = static_cast<int>((delayMs + depthMs) * sampleRate / 1000.0f);

    state.delayBuffer.resize(delayBufferSize, 0.0f);
    state.writeIndex = 0;

    for (size_t i = 0; i < samples.size(); ++i)
    {

        float currentDelayMs = delayMs + depthMs * sinf(2.0f * M_PI * state.phase);
        float currentDelaySamples = currentDelayMs * sampleRate / 1000.0f;

        float readPos = state.writeIndex - currentDelaySamples;
        if (readPos < 0)
            readPos += delayBufferSize;

        int readIndex = static_cast<int>(readPos);
        float frac = readPos - readIndex;

        int readIndex2 = (readIndex + 1) % delayBufferSize;
        readIndex = readIndex % delayBufferSize;

        float delayedSample = state.delayBuffer[readIndex] * (1.0f - frac) + state.delayBuffer[readIndex2] * frac;

        state.delayBuffer[state.writeIndex] = samples[i];

        samples[i] = 0.5f * (samples[i] + delayedSample);

        state.writeIndex = (state.writeIndex + 1) % delayBufferSize;

        state.phase += lfoFrequency;
        if (state.phase >= 1.0f)
            state.phase -= 1.0f;
    }
}
