#ifndef REVERBERATION_H
#define REVERBERATION_H

#include <stdint.h>
#include "ezdsp5502.h"

void combFilter(const float* inputSamples, float* outputSamples, int numSamples, float delayMs, float decayGain, float sampleRate);
void allPassFilter(const float* inputSamples, float* outputSamples, int numSamples, float delayMs, float decayGain, float sampleRate);
void schroederReverb(const float* inputSamples, float* outputSamples, int numSamples, float sampleRate);
void mixDryWet(const float* dry, const float* wet, float* output, int numSamples, float wetLevel);

#endif // REVERBERATION_H
