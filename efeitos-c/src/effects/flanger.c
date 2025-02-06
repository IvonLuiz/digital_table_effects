#define _USE_MATH_DEFINES

#include "effects/flanger.h"
#include <math.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

void apply_flanger(int16_t *samples, size_t numSamples, int sampleRate, float delayMs, float depthMs, float rateHz)
{
  // Initialize flanger state
  FlangerState state;
  state.phase = 0.0f;
  state.writeIndex = 0;

  // Calculate buffer size based on maximum possible delay
  int delayBufferSize = (int)((delayMs + depthMs) * sampleRate / 1000.0f);

  // Allocate memory for delay buffer (now using int16_t for 16-bit samples)
  state.delayBuffer = (int16_t *)malloc(delayBufferSize * sizeof(int16_t));
  if (state.delayBuffer == NULL)
  {
    printf("Memory allocation failed for delay buffer!\n");
    return;
  }

  // Initialize delay buffer to zero
  memset(state.delayBuffer, 0, delayBufferSize * sizeof(int16_t));

  // Calculate LFO frequency in radians per sample
  float lfoFrequency = 2.0f * M_PI * rateHz / sampleRate;

  // Process each sample
  for (size_t i = 0; i < numSamples; ++i)
  {
    // Calculate current delay using LFO
    float currentDelayMs = delayMs + depthMs * sinf(state.phase);
    float currentDelaySamples = currentDelayMs * sampleRate / 1000.0f;

    // Get delay read position (circular buffer)
    float readPos = (float)state.writeIndex - currentDelaySamples;
    if (readPos < 0.0f)
      readPos += delayBufferSize;

    // Linear interpolation of delayed signal to avoid artifacts from fractional delay times
    int readIndex1 = (int)readPos % delayBufferSize;
    int readIndex2 = (readIndex1 + 1) % delayBufferSize;
    float frac = readPos - floorf(readPos);

    // Interpolate between the two buffer samples
    float delayedSample = state.delayBuffer[readIndex1] * (1.0f - frac) + state.delayBuffer[readIndex2] * frac;

    // Mix the original and delayed signal, scaling the result back to int16_t
    int16_t mixedSample = (int16_t)(0.5f * (samples[i] + delayedSample));

    // Write input sample to buffer
    state.delayBuffer[state.writeIndex] = samples[i];

    // Update the sample with the mixed output
    samples[i] = mixedSample;

    // Update write index and phase
    state.writeIndex = (state.writeIndex + 1) % delayBufferSize;
    state.phase += lfoFrequency;
    if (state.phase >= 2.0f * M_PI)
      state.phase -= 2.0f * M_PI;
  }

  // Free the allocated memory for delay buffer
  free(state.delayBuffer);
}