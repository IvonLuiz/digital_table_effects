#define _USE_MATH_DEFINES

#include "effects/flanger.h"
#include <cmath>
#include <vector>
#include <algorithm>
#include <iostream>

struct FlangerState {
    std::vector<float> delayBuffer;
    float phase;
    int writeIndex;
};

void flanger(std::vector<float>& samples, int sampleRate, float delayMs, float depthMs, float rateHz) {
    // Initialize flanger state
    FlangerState state;
    state.phase = 0.0f;
    float lfoFrequency = rateHz / sampleRate;
    
    // Calculate buffer size based on maximum possible delay
    int delayBufferSize = static_cast<int>((delayMs + depthMs) * sampleRate / 1000.0f);
    // The delay buffer acts as a circular buffer using modulo operations
    state.delayBuffer.resize(delayBufferSize, 0.0f);
    state.writeIndex = 0;

    // Process each sample
    for (size_t i = 0; i < samples.size(); ++i) {
        // Calculate current delay using LFO
        float currentDelayMs = delayMs + depthMs * sinf(2.0f * M_PI * state.phase);
        float currentDelaySamples = currentDelayMs * sampleRate / 1000.0f;

        // Get delay read position with linear interpolation
        float readPos = state.writeIndex - currentDelaySamples;
        if (readPos < 0) readPos += delayBufferSize;
        
        int readIndex = static_cast<int>(readPos);
        float frac = readPos - readIndex;
        
        // Ensure indices are within bounds
        int readIndex2 = (readIndex + 1) % delayBufferSize;
        readIndex = readIndex % delayBufferSize;

        // Linear interpolation of delayed signal helps avoid artifacts from fractional delay times
        float delayedSample = state.delayBuffer[readIndex] * (1.0f - frac) + state.delayBuffer[readIndex2] * frac;

        // Write input to buffer
        state.delayBuffer[state.writeIndex] = samples[i];
        
        // Mix original and delayed signal
        samples[i] = 0.5f * (samples[i] + delayedSample);

        // Update indices and phase
        state.writeIndex = (state.writeIndex + 1) % delayBufferSize;
        // The phase accumulator wraps between 0 and 1 to create a continuous LFO cycle
        state.phase += lfoFrequency;
        if (state.phase >= 1.0f) state.phase -= 1.0f;
    }
}
