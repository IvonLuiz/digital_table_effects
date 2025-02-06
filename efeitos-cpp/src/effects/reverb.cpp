#include "effects/reverb.h"

std::vector<float> combFilter(const std::vector<float> &inputSamples, float delayMs, float decayGain, float sampleRate)
{
    int delaySamples = static_cast<int>(delayMs * sampleRate / 1000);
    std::vector<float> combFilterSamples(inputSamples.size());

    for (size_t i = 0; i < inputSamples.size(); ++i)
    {
        combFilterSamples[i] = inputSamples[i];
        if (i >= delaySamples)
        {
            combFilterSamples[i] += combFilterSamples[i - delaySamples] * decayGain;
        }
    }

    return combFilterSamples;
}

std::vector<float> allPassFilter(const std::vector<float> &inputSamples, float delayMs, float decayGain, float sampleRate)
{
    int delaySamples = static_cast<int>(delayMs * sampleRate / 1000);
    std::vector<float> allPassFilterSamples(inputSamples.size());
    std::vector<float> buffer(delaySamples, 0);

    for (size_t i = 0; i < inputSamples.size(); ++i)
    {
        float delayedSample = (i >= delaySamples) ? allPassFilterSamples[i - delaySamples] : buffer[i];
        allPassFilterSamples[i] = inputSamples[i] + decayGain * delayedSample;
        if (i >= delaySamples)
        {
            allPassFilterSamples[i] -= decayGain * inputSamples[i - delaySamples];
        }
    }

    return allPassFilterSamples;
}

std::vector<float> schroederReverb(const std::vector<float> &inputSamples, float sampleRate)
{

    std::vector<float> combDelays = {29.7f, 18.0f, 45.0f, 60.0f};
    std::vector<float> combGains = {0.805f, 0.827f, 0.783f, 0.764f};

    std::vector<float> combOutput(inputSamples.size(), 0.0f);
    for (size_t i = 0; i < combDelays.size(); ++i)
    {
        std::vector<float> combSamples = combFilter(inputSamples, combDelays[i], combGains[i], sampleRate);
        for (size_t j = 0; j < combOutput.size(); ++j)
        {
            combOutput[j] += combSamples[j];
        }
    }

    std::vector<float> allPassDelays = {5.0f, 1.7f};
    std::vector<float> allPassGains = {0.7f, 0.7f};

    std::vector<float> allPassOutput = combOutput;
    for (size_t i = 0; i < allPassDelays.size(); ++i)
    {
        allPassOutput = allPassFilter(allPassOutput, allPassDelays[i], allPassGains[i], sampleRate);
    }

    return allPassOutput;
}

std::vector<float> mixDryWet(const std::vector<float> &dry, const std::vector<float> &wet, float wetLevel)
{
    std::vector<float> output(dry.size());
    for (size_t i = 0; i < dry.size(); ++i)
    {
        output[i] = (1.0f - wetLevel) * dry[i] + wetLevel * wet[i];
    }
    return output;
}