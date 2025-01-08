#include <stdlib.h>
#include "reverberation.h"

// Comb Filter
void combFilter(const float* inputSamples, float* outputSamples, int numSamples, float delayMs, float decayGain, float sampleRate) {
    int delaySamples = (int)(delayMs * sampleRate / 1000);
    int i;
    for (i = 0; i < numSamples; ++i) {
        outputSamples[i] = inputSamples[i];
        if (i >= delaySamples) {
            outputSamples[i] += outputSamples[i - delaySamples] * decayGain;
        }
    }
}

// All-Pass Filter
void allPassFilter(const float* inputSamples, float* outputSamples, int numSamples, float delayMs, float decayGain, float sampleRate) {
    int delaySamples = (int)(delayMs * sampleRate / 1000);
    float* buffer = (float*)calloc(delaySamples, sizeof(float));
    int i;
    for (i = 0; i < numSamples; ++i) {
        float delayedSample = (i >= delaySamples) ? outputSamples[i - delaySamples] : buffer[i];
        outputSamples[i] = inputSamples[i] + decayGain * delayedSample;
        if (i >= delaySamples) {
            outputSamples[i] -= decayGain * inputSamples[i - delaySamples];
        }
    }
    free(buffer);
}

// Schroeder Reverberator
void schroederReverb(const float* inputSamples, float* outputSamples, int numSamples, float sampleRate) {
    // Parameters for comb filters
    float combDelays[] = {29.7f, 18.0f, 45.0f, 60.0f}; // in milliseconds
    float combGains[] = {0.805f, 0.827f, 0.783f, 0.764f};

    // Apply comb filters in parallel
    float* combOutput = (float*)calloc(numSamples, sizeof(float));
    int i, j;
    for (i = 0; i < 4; ++i) {
        float* combSamples = (float*)calloc(numSamples, sizeof(float));
        combFilter(inputSamples, combSamples, numSamples, combDelays[i], combGains[i], sampleRate);
        for (j = 0; j < numSamples; ++j) {
            combOutput[j] += combSamples[j];
        }
        free(combSamples);
    }

    // Parameters for all-pass filters
    float allPassDelays[] = {5.0f, 1.7f}; // in milliseconds
    float allPassGains[] = {0.7f, 0.7f};

    // Apply all-pass filters in series
    float* allPassOutput = combOutput;
    for (i = 0; i < 2; ++i) {
        float* tempOutput = (float*)calloc(numSamples, sizeof(float));
        allPassFilter(allPassOutput, tempOutput, numSamples, allPassDelays[i], allPassGains[i], sampleRate);
        if (i > 0) {
            free(allPassOutput);
        }
        allPassOutput = tempOutput;
    }

    for (i = 0; i < numSamples; ++i) {
        outputSamples[i] = allPassOutput[i];
    }

    free(allPassOutput);
}

// Mix dry and wet signals
void mixDryWet(const float* dry, const float* wet, float* output, int numSamples, float wetLevel) {
    int i;
    for (i = 0; i < numSamples; ++i) {
        output[i] = (1.0f - wetLevel) * dry[i] + wetLevel * wet[i];
    }
}
