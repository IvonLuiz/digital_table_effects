#include "reverb.h"
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <math.h>
#include "tistdtypes.h"

float band_pass_filter(float sample, float centerFreq, float q,
                       float sampleRate, float *state)
{
  float omega = 2.0f * PI * centerFreq / sampleRate;
  float alpha = sinf(omega) / (2.0f * q);

  float a0 = 1.0f + alpha;
  float b0 = alpha;
  float b1 = 0.0f;
  float b2 = -alpha;
  float a1 = -2.0f * cosf(omega);
  float a2 = 1.0f - alpha;

  float output = (b0 / a0) * sample + state[0];
  state[0] = (b1 / a0) * sample + state[1] - (a1 / a0) * output;
  state[1] = (b2 / a0) * sample - (a2 / a0) * output;

  return output;
}

void all_pass_filter(Int16 *samples, size_t numSamples, float delayMs, float decayGain, int sampleRate)
{
  int delaySamples = (int)(delayMs * sampleRate / 1000);
  Int16 *buffer = (Int16 *)malloc(delaySamples * sizeof(Int16));
  if (!buffer)
    return;

  memset(buffer, 0, delaySamples * sizeof(Int16));
  size_t i;
  for (; i < numSamples; ++i)
  {
    int delayedIndex = i % delaySamples;
    float delayedSample = buffer[delayedIndex];
    buffer[delayedIndex] = samples[i] + decayGain * delayedSample;
    samples[i] = (Int16)((1 - decayGain) * delayedSample + samples[i]);
  }

  free(buffer);
}

void comb_filter(Int16 *samples, size_t numSamples, float delayMs, float decayGain, int sampleRate)
{
  int delaySamples = (int)(delayMs * sampleRate / 1000);
  Int16 *buffer = (Int16 *)malloc(delaySamples * sizeof(Int16));
  if (!buffer)
    return;

  memset(buffer, 0, delaySamples * sizeof(Int16));
  size_t i = 0;
  for (; i < numSamples; ++i)
  {
    int delayedIndex = i % delaySamples;
    samples[i] += (Int16)(buffer[delayedIndex] * decayGain);
    buffer[delayedIndex] = samples[i];
  }

  free(buffer);
}

void reverse_samples(int16_t *data, uint32_t numSamples)
{
  uint32_t i = 0;
  for (; i < numSamples / 2; i++)
  {
    Int16 temp = data[i];
    data[i] = data[numSamples - i - 1];
    data[numSamples - i - 1] = temp;
  }
}

void apply_reverb_simple(int16_t *data, uint32_t numSamples, uint32_t sampleRate, float delay, float decay)
{

  int delaySamples = (int)(delay * sampleRate);
  printf("Delay Samples: %d, Decay: %.2f\n", delaySamples, decay);

  uint32_t i = 0;
  for (; i < numSamples; i++)
  {
    int32_t delayedSample = 0;

    if (i >= (uint32_t)delaySamples)
    {
      delayedSample = (int32_t)(data[i - delaySamples] * decay);
    }

    int32_t mixedSample = (int32_t)data[i] + delayedSample;

    if (mixedSample > INT16_MAX)
      mixedSample = INT16_MAX;
    else if (mixedSample < INT16_MIN)
      mixedSample = INT16_MIN;

    data[i] = (int16_t)mixedSample;
  }
}
void apply_reverb_hall(int16_t *data, uint32_t numSamples, uint32_t sampleRate, float delay, float decay)
{
  reverse_samples(data, numSamples);
  apply_reverb_simple(data, numSamples, sampleRate, delay, decay);
  reverse_samples(data, numSamples);
}

void apply_reverb_shroeder(Int16 *samples, size_t numSamples, int sampleRate)
{

  float combDelays[] = {29.7f, 18.0f, 45.0f, 60.0f};
  float combGains[] = {0.805f, 0.827f, 0.783f, 0.764f};
  int i = 0;
  for (; i < 4; ++i)
  {
    comb_filter(samples, numSamples, combDelays[i], combGains[i], sampleRate);
  }

  float allPassDelays[] = {5.0f, 1.7f};
  float allPassGains[] = {0.7f, 0.7f};
  int j = 0;
  for (; j < 2; ++j)
  {
    all_pass_filter(samples, numSamples, allPassDelays[j], allPassGains[j], sampleRate);
  }
}
