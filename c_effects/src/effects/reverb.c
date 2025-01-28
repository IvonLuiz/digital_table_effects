#include "effects/reverb.h"
#include <stdio.h>

// Band-pass filter
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

// Function to reverse audio samples
void reverse_samples(int16_t *data, uint32_t numSamples)
{
  for (uint32_t i = 0; i < numSamples / 2; i++)
  {
    int16_t temp = data[i];
    data[i] = data[numSamples - i - 1];
    data[numSamples - i - 1] = temp;
  }
}

// Apply a simple reverb effect
void apply_reverb(int16_t *data, uint32_t numSamples, int delay, float decay)
{
  for (uint32_t i = delay; i < numSamples; i++)
  {
    data[i] += (int16_t)(data[i - delay] * decay);
  }
}

void apply_rev_hall(int16_t *data, uint32_t numSamples, uint32_t sampleRate)
{
  reverse_samples(data, numSamples);
  int delay = 0.5f * sampleRate;
  float decay = 0.5f;
  apply_reverb(data, numSamples, delay, decay);
  reverse_samples(data, numSamples);
}
