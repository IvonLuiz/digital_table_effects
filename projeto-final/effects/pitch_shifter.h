#pragma once

#ifndef PITCH_SHIFTER_H
#define PITCH_SHIFTER_H

#include <stddef.h>
#include <stdint.h>
#include "tistdtypes.h"

#define WINDOW_SIZE 0.03f

/**
 * Structure to hold the pitch shifter state.
 */
typedef struct
{
  float phase;        /**< Phase of the pitch modulation. */
  size_t writeIndex;  /**< Write index for the delay buffer. */
  Int16 *delayBuffer; /**< Pointer to the dynamically allocated delay buffer. */
} PitchShifterState;

/**
 * Generates a sawtooth wave value.
 * @param phase - The phase of the wave (0 to 1).
 * @param amplitude - The amplitude of the wave.
 * @return The computed sawtooth wave value.
 */
float sawtooth(float phase, float amplitude);

/**
 * Computes an envelope function for smoothing transitions.
 * @param phase - The phase of the envelope (0 to 1).
 * @return The computed envelope value.
 */
float envelope(float phase);

/**
 * Applies pitch shifting to an audio signal using two variable delay lines with linear interpolation.
 * @param input - Pointer to the input audio samples.
 * @param output - Pointer to the output buffer where processed samples will be stored.
 * @param length - Number of samples in the input signal.
 * @param sampleRate - The sample rate of the audio signal.
 * @param pitchShift - The pitch shift factor.
 */
void apply_pitch_shifter(Int16 *samples, size_t numSamples, int sampleRate, float pitchShift);

#endif // PITCH_SHIFTER_H
