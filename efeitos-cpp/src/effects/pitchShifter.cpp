#define _USE_MATH_DEFINES

#include "effects/pitchShifter.h"
#include <cmath>
#include <algorithm>
#include <iostream>

float sawtooth(float phase, float amplitude)
{
    return amplitude * (2.0f * (phase - floor(phase)) - 1.0f);
}

float envelope(float phase)
{
    float t = fmod(phase, 1.0f);
    return t < 0.5f ? 2.0f * t : 2.0f * (1.0f - t);
}

void shiftPitch(const std::vector<float> &input, std::vector<float> &output, int sampleRate, float pitchShift)
{

    const float windowSize = 0.03f;
    const float frequency = sampleRate / windowSize;
    const int delaySamples = static_cast<int>(sampleRate * windowSize);
    const float phaseIncrement = pitchShift / sampleRate;

    std::vector<float> delayBuffer(delaySamples, 0.0f);
    size_t delayIndex = 0;
    float phase1 = 0.0f;
    float phase2 = 0.5f;

    output.resize(input.size());
    for (size_t i = 0; i < input.size(); ++i)
    {

        float delay1 = sawtooth(phase1, delaySamples);
        float delay2 = sawtooth(phase2, delaySamples);

        int index1 = static_cast<int>(floor(delayIndex - delay1)) % delaySamples;
        int index2 = static_cast<int>(floor(delayIndex - delay2)) % delaySamples;

        if (index1 < 0)
            index1 += delaySamples;
        if (index2 < 0)
            index2 += delaySamples;

        float sample1 = delayBuffer[index1];
        float sample2 = delayBuffer[index2];

        float env1 = envelope(phase1);
        float env2 = envelope(phase2);

        output[i] = env1 * sample1 + env2 * sample2;

        delayBuffer[delayIndex] = input[i];
        delayIndex = (delayIndex + 1) % delaySamples;

        phase1 = fmod(phase1 + phaseIncrement, 1.0f);
        phase2 = fmod(phase2 + phaseIncrement, 1.0f);
    }
}
