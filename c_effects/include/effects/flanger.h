#pragma once
#ifndef FLANGER_H
#define FLANGER_H

#include <stddef.h> // for size_t
#include <stdint.h> // for int16_t

#define PI 3.141592653589793

// Define the FlangerState struct in the header file with typedef
typedef struct
{
  int16_t *delayBuffer; // Delay buffer for storing previous samples (int16_t instead of float)
  float phase;          // Phase of the low-frequency oscillator (LFO)
  int writeIndex;       // Index for writing new samples to the buffer
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