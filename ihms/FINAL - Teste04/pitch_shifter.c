#include "pitch_shifter.h"

#include <math.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

float sawtooth(float phase, float amplitude)
{
  return amplitude * (2.0f * (phase - floorf(phase)) - 1.0f);
}

float envelope(float phase)
{
  float t = fmodf(phase, 1.0f);
  return (t < 0.5f) ? (2.0f * t) : (2.0f * (1.0f - t));
}

void apply_pitch_shifter(Int16 *samples, size_t numSamples, int sampleRate, float pitchShift)
{
  PitchShifterState state;
  state.phase = 0.0f;
  state.writeIndex = 0;

  float windowSize = 0.03f; // 30ms window
  int delayBufferSize = (int)(sampleRate * windowSize);
  state.delayBuffer = (Int16 *)malloc(delayBufferSize * sizeof(Int16));
  if (!state.delayBuffer)
  {
    printf("Memory allocation failed for delay buffer!\n");
    return;
  }
  memset(state.delayBuffer, 0, delayBufferSize * sizeof(Int16));

  float phaseIncrement = pitchShift / sampleRate;
  float phase2 = 0.5f;

  size_t i = 0;
  for (; i < numSamples; ++i)
  {
    float delay1 = sawtooth(state.phase, (float)delayBufferSize);
    float delay2 = sawtooth(phase2, (float)delayBufferSize);

    float readPos1 = (float)state.writeIndex - delay1;
    float readPos2 = (float)state.writeIndex - delay2;

    if (readPos1 < 0.0f)
      readPos1 += delayBufferSize;
    if (readPos2 < 0.0f)
      readPos2 += delayBufferSize;

    int index1 = (int)readPos1 % delayBufferSize;
    int index2 = (int)readPos2 % delayBufferSize;

    int nextIndex1 = (index1 + 1) % delayBufferSize;
    int nextIndex2 = (index2 + 1) % delayBufferSize;

    float frac1 = readPos1 - floorf(readPos1);
    float frac2 = readPos2 - floorf(readPos2);

    float sample1 = state.delayBuffer[index1] * (1.0f - frac1) + state.delayBuffer[nextIndex1] * frac1;
    float sample2 = state.delayBuffer[index2] * (1.0f - frac2) + state.delayBuffer[nextIndex2] * frac2;

    float env1 = envelope(state.phase);
    float env2 = envelope(phase2);

    Int16 mixedSample = (Int16)(env1 * sample1 + env2 * sample2);

    state.delayBuffer[state.writeIndex] = samples[i];
    samples[i] = mixedSample;

    state.writeIndex = (state.writeIndex + 1) % delayBufferSize;
    state.phase = fmodf(state.phase + phaseIncrement, 1.0f);
    phase2 = fmodf(phase2 + phaseIncrement, 1.0f);
  }

  free(state.delayBuffer);
}
