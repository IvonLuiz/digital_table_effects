#define _USE_MATH_DEFINES

#include "effects/pitchShifter.h"
#include <cmath>
#include <algorithm>
#include <iostream>


// Sawtooth wave is used to generate the delay times
float sawtooth(float phase, float amplitude) {
    return amplitude * (2.0f * (phase - floor(phase)) - 1.0f);
}

// Função para calcular a envolvente (fade-in e fade-out para suavizar transições)
// A good choice of envelope is one half cycle of a sinusoid. 
// If we assume on average that the two delay outputs are uncorrelated,
// the signal power from the two delay lines, after enveloping, will add to a
// constant (since the sum of squares of the two envelopes is one).
float envelope(float phase) {
    float t = fmod(phase, 1.0f);
    return t < 0.5f ? 2.0f * t : 2.0f * (1.0f - t);
}

// Pitch shifting using two variable delay lines with linear interpolation
void shiftPitch(const std::vector<float>& input, std::vector<float>& output, int sampleRate, float pitchShift) {

    // Pitch shifter settings
    const float windowSize = 0.03f; // 30ms window (adjustable)
    const float frequency = sampleRate / windowSize;
    const int delaySamples = static_cast<int>(sampleRate * windowSize);
    const float phaseIncrement = pitchShift / sampleRate;

    std::vector<float> delayBuffer(delaySamples, 0.0f);
    size_t delayIndex = 0;
    float phase1 = 0.0f; // Phase of the first delay line
    float phase2 = 0.5f; // Phase of the second delay line (180 degrees out of phase)

    // Process the input samples
    output.resize(input.size());
    for (size_t i = 0; i < input.size(); ++i) {
        // Generate delay times based on the sawtooth wave
        float delay1 = sawtooth(phase1, delaySamples);
        float delay2 = sawtooth(phase2, delaySamples);

        // Calculate the indices in the delay line
        int index1 = static_cast<int>(floor(delayIndex - delay1)) % delaySamples;
        int index2 = static_cast<int>(floor(delayIndex - delay2)) % delaySamples;

        if (index1 < 0) index1 += delaySamples;
        if (index2 < 0) index2 += delaySamples;

        // Linear interpolation to smooth
        float sample1 = delayBuffer[index1];
        float sample2 = delayBuffer[index2];

        // Apply the envelopes
        float env1 = envelope(phase1);
        float env2 = envelope(phase2);

        // Combine the two delays with envelopes
        output[i] = env1 * sample1 + env2 * sample2;

        // Update the delay buffer
        delayBuffer[delayIndex] = input[i];
        delayIndex = (delayIndex + 1) % delaySamples;

        // Increment the phases and normalize
        phase1 = fmod(phase1 + phaseIncrement, 1.0f);
        phase2 = fmod(phase2 + phaseIncrement, 1.0f);
    }
}
