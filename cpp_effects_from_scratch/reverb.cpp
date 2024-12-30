#include <iostream>
#include <vector>
#include <algorithm>
#include "reverb.h"

// Comb Filter
std::vector<int16_t> combFilter(const std::vector<int16_t>& inputSamples, double delayMs, double decayGain, float sampleRate) {
    int delaySamples = static_cast<int>(delayMs * sampleRate / 1000);
    std::vector<int16_t> combFilterSamples(inputSamples.size());

    for (size_t i = 0; i < inputSamples.size(); ++i) {
        combFilterSamples[i] = inputSamples[i];
        if (i >= delaySamples) {
            combFilterSamples[i] += static_cast<int16_t>(combFilterSamples[i - delaySamples] * decayGain);
        }
    }

    return combFilterSamples;
}

// All-Pass Filter
std::vector<int16_t> allPassFilter(const std::vector<int16_t>& inputSamples, double delayMs, double decayGain, float sampleRate) {
    int delaySamples = static_cast<int>(delayMs * sampleRate / 1000);
    std::vector<int16_t> allPassFilterSamples(inputSamples.size());
    std::vector<int16_t> buffer(delaySamples, 0);

    for (size_t i = 0; i < inputSamples.size(); ++i) {
        int delayedSample = (i >= delaySamples) ? allPassFilterSamples[i - delaySamples] : buffer[i];
        allPassFilterSamples[i] = inputSamples[i] + static_cast<int16_t>(decayGain * delayedSample);
        if (i >= delaySamples) {
            allPassFilterSamples[i] -= static_cast<int16_t>(decayGain * inputSamples[i - delaySamples]);
        }
    }

    return allPassFilterSamples;
}

// Schroeder Reverberator
std::vector<int16_t> schroederReverb(const std::vector<int16_t>& inputSamples, float sampleRate) {
    // Parameters for comb filters
    std::vector<double> combDelays = {29.7, 37.1, 41.1, 43.7}; // in milliseconds
    std::vector<double> combGains = {0.805, 0.827, 0.783, 0.764};

    // Parameters for all-pass filters
    std::vector<double> allPassDelays = {5.0, 1.7}; // in milliseconds
    std::vector<double> allPassGains = {0.7, 0.7};

    // Apply comb filters
    std::vector<int16_t> combOutput = inputSamples;
    for (size_t i = 0; i < combDelays.size(); ++i) {
        combOutput = combFilter(combOutput, combDelays[i], combGains[i], sampleRate);
    }

    // Apply all-pass filters
    std::vector<int16_t> allPassOutput = combOutput;
    for (size_t i = 0; i < allPassDelays.size(); ++i) {
        allPassOutput = allPassFilter(allPassOutput, allPassDelays[i], allPassGains[i], sampleRate);
    }

    return allPassOutput;
}

