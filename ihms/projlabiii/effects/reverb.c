#include "reverb.h"
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <math.h>
#include "tistdtypes.h"

// Apply band-pass filter
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

  // Apply filter
  float output = (b0 / a0) * sample + state[0];
  state[0] = (b1 / a0) * sample + state[1] - (a1 / a0) * output;
  state[1] = (b2 / a0) * sample - (a2 / a0) * output;

  return output;
}

// Apply all-pass filter
void all_pass_filter(Int16 *samples, size_t numSamples, float delayMs, float decayGain, int sampleRate)
{
  int delaySamples = (int)(delayMs * sampleRate / 1000);
  Int16 *buffer = (Int16 *)malloc(delaySamples * sizeof(Int16));
  if (!buffer)
    return;
  int i = 0;
  for (; i < delaySamples; ++i)
    buffer[i] = 0;

  int j = 0;
  for (; j < numSamples; ++j)
  {
    int delayedIndex = i % delaySamples;
    float delayedSample = buffer[delayedIndex];
    samples[i] = samples[i] + decayGain * delayedSample - decayGain * samples[i];
    buffer[delayedIndex] = samples[i];
  }
  free(buffer);
}

// Apply comb filter
void comb_filter(Int16 *samples, size_t numSamples, float delayMs, float decayGain, int sampleRate)
{
  int delaySamples = (int)(delayMs * sampleRate / 1000);
  Int16 *buffer = (Int16 *)malloc(delaySamples * sizeof(Int16));
  if (!buffer)
    return;
  int i = 0;
  for (; i < delaySamples; ++i)
    buffer[i] = 0;

  int j = 0;
  for (; j < numSamples; ++j)
  {
    int delayedIndex = i % delaySamples;
    samples[j] += buffer[delayedIndex] * decayGain;
    buffer[delayedIndex] = samples[j];
  }
  free(buffer);
}

// Function to reverse audio samples
void reverse_samples(Int16 *data, uint32_t numSamples)
{
  uint32_t i = 0;
  for (; i < numSamples / 2; i++)
  {
    Int16 temp = data[i];
    data[i] = data[numSamples - i - 1];
    data[numSamples - i - 1] = temp;
  }
}

// Apply a simple reverb
void apply_reverb_simple(Int16 *data, uint32_t numSamples, uint32_t sampleRate, float delay, float decay)
{
  int delay_ms = delay * sampleRate;
  uint32_t i = delay_ms;
  for (; i < numSamples; i++)
  {
    data[i] += (Int16)(data[i - delay_ms] * decay);
  }
}

// Apply hall reverb
void apply_reverb_hall(Int16 *data, uint32_t numSamples, uint32_t sampleRate, float delay, float decay)
{
  reverse_samples(data, numSamples);
  int delay_ms = delay * sampleRate;
  apply_reverb_simple(data, numSamples, sampleRate, delay_ms, decay);
  reverse_samples(data, numSamples);
}

// Apply Schroeder reverb
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
    all_pass_filter(samples, numSamples, allPassDelays[i], allPassGains[i], sampleRate);
  }
}

// Mix dry and wet signals
void mix_dry_wet(Int16 *dry, Int16 *wet, float *output, size_t numSamples, float wetLevel)
{
  int i = 0;
  for (; i < numSamples; ++i)
  {
    output[i] = (1.0f - wetLevel) * dry[i] + wetLevel * wet[i];
  }
}
