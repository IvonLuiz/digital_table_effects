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

