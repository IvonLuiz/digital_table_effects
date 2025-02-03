#define _USE_MATH_DEFINES

#include "flanger.h"
#include <math.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "tistdtypes.h"

#define MAX_DELAY_MS 50 // Define a reasonable maximum delay in milliseconds

void apply_flanger(int16_t *samples, size_t numSamples, int sampleRate, float delayMs, float depthMs, float rateHz)
{
  // Define a static delay buffer to avoid dynamic memory allocation
  static int16_t delayBuffer[MAX_DELAY_MS * 48]; // Max 48kHz sample rate, 50ms max delay
  static size_t delayBufferSize = 0;
  static size_t writeIndex = 0;
  static float phase = 0.0f;

  // Initialize the delay buffer size
  if (delayBufferSize == 0)
  {
    delayBufferSize = (int)((delayMs + depthMs) * sampleRate / 1000.0f);
    memset(delayBuffer, 0, sizeof(delayBuffer)); // Zero out the buffer initially
  }

  // Calculate constants
  float maxDelaySamples = (delayMs + depthMs) * sampleRate / 1000.0f;
  float lfoFrequency = 2.0f * M_PI * rateHz / sampleRate;

  // Process each audio sample
  size_t i = 0;
  for (; i < numSamples; ++i)
  {
    // Calculate the current LFO modulated delay in samples
    float currentDelaySamples = delayMs * sampleRate / 1000.0f + depthMs * sampleRate / 1000.0f * sinf(phase);

    // Get the read position in the delay buffer
    float readPos = (float)writeIndex - currentDelaySamples;
    while (readPos < 0) // Wrap the read position
      readPos += delayBufferSize;

    // Perform linear interpolation for smoother delay output
    int readIndex1 = (int)readPos;
    int readIndex2 = (readIndex1 + 1) % delayBufferSize;
    float frac = readPos - readIndex1;

    // Get interleaved samples
    float delayedSample = (1.0f - frac) * delayBuffer[readIndex1] + frac * delayBuffer[readIndex2];

    // Mix the original and delayed signal
    int32_t mixedSample = (int32_t)(0.5f * samples[i] + 0.5f * delayedSample);

    // Clamping to Int16 range
    if (mixedSample > 32767)
      mixedSample = 32767;
    else if (mixedSample < -32768)
      mixedSample = -32768;

    // Write the mixed sample back
    samples[i] = (int16_t)mixedSample;

    // Store the current sample in the delay buffer
    delayBuffer[writeIndex] = samples[i];

    // Update the write index and LFO phase
    writeIndex = (writeIndex + 1) % delayBufferSize;
    phase += lfoFrequency;
    if (phase >= 2.0f * M_PI)
      phase -= 2.0f * M_PI;
  }
}
