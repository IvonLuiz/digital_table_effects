#pragma once
#ifndef FLANGER_H
#define FLANGER_H

#include <stddef.h>
#include <stdint.h>

#define PI 3.141592653589793

typedef struct
{
  int16_t *delayBuffer;
  float phase;
  int writeIndex;
} FlangerState;

/**
 * Applies the flanger effect to the given audio samples.
 *
 * @param samples       Array of audio samples to apply the effect to (int16_t).
 * @param numSamples    Number of samples in the `samples` array.
 * @param sampleRate    The sample rate of the audio (e.g., 44100 Hz).
 * @param delayMs       Delay time in milliseconds.
 * @param depthMs       Depth of the modulation in milliseconds.
 * @param rateHz        Rate of the LFO in Hertz.
 */
void apply_flanger(int16_t *samples, size_t numSamples, int sampleRate, float delayMs, float depthMs, float rateHz);

#endif // FLANGER_H