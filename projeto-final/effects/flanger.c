#define _USE_MATH_DEFINES

#include "flanger.h"
#include <math.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "tistdtypes.h"

#define MAX_DELAY_MS 50

void apply_flanger(int16_t *samples, size_t numSamples, int sampleRate, float delayMs, float depthMs, float rateHz)
{

  static int16_t delayBuffer[MAX_DELAY_MS * 48];
  static size_t delayBufferSize = 0;
  static size_t writeIndex = 0;
  static float phase = 0.0f;

  if (delayBufferSize == 0)
  {
    delayBufferSize = (int)((delayMs + depthMs) * sampleRate / 1000.0f);
    memset(delayBuffer, 0, sizeof(delayBuffer));
  }

  float lfoFrequency = 2.0f * M_PI * rateHz / sampleRate;

  size_t i = 0;
  for (; i < numSamples; ++i)
  {

    float currentDelaySamples = delayMs * sampleRate / 1000.0f + depthMs * sampleRate / 1000.0f * sinf(phase);

    float readPos = (float)writeIndex - currentDelaySamples;
    while (readPos < 0)
      readPos += delayBufferSize;

    int readIndex1 = (int)readPos;
    int readIndex2 = (readIndex1 + 1) % delayBufferSize;
    float frac = readPos - readIndex1;

    float delayedSample = (1.0f - frac) * delayBuffer[readIndex1] + frac * delayBuffer[readIndex2];

    int32_t mixedSample = (int32_t)(0.5f * samples[i] + 0.5f * delayedSample);

    if (mixedSample > 32767)
      mixedSample = 32767;
    else if (mixedSample < -32768)
      mixedSample = -32768;

    samples[i] = (int16_t)mixedSample;

    delayBuffer[writeIndex] = samples[i];

    writeIndex = (writeIndex + 1) % delayBufferSize;
    phase += lfoFrequency;
    if (phase >= 2.0f * M_PI)
      phase -= 2.0f * M_PI;
  }
}
