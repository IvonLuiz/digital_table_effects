#include <math.h>
#include <stdint.h>
#include <stdlib.h>
#include "tistdtypes.h"

static float fmaxf(float a, float b)
{
  return (a > b) ? a : b;
}

static float fminf(float a, float b)
{
  return (a < b) ? a : b;
}

static float clip(float n, float lower, float upper)
{
  return fmaxf(lower, fminf(n, upper));
}

void apply_tremolo(int16_t *samples, size_t num_samples, int sampleRate, uint32_t milliseconds, uint32_t *position)
{
  uint32_t half_cycle_samples = (milliseconds * sampleRate) / 1000 / 2;
  float depth = 1.0f;
  int waveform = 1;
  size_t i;

  for (i = 0; i < num_samples; ++i)
  {
    float waveform_state = 0.0f;
    float triangle_result = 0.0f;
    float relative_pos = 0.0f;

    if (*position >= half_cycle_samples)
    {
      *position = 0;
    }

    relative_pos = (2.0f * (*position) / (float)half_cycle_samples) - 1.0f;
    relative_pos = clip(relative_pos, -1.0f, 1.0f);
    triangle_result = relative_pos;

    if (waveform == 0)
    {

      waveform_state = (*position < (half_cycle_samples / 2)) ? -1.0f : 1.0f;
    }
    else if (waveform == 1)
    {

      waveform_state = triangle_result;
    }
    else if (waveform == 2)
    {

      waveform_state = 1.0f - (triangle_result * triangle_result);
    }

    float amplitude = ((waveform_state + 1.0f) / 2.0f) * depth;

    samples[i] = (int16_t)(samples[i] * amplitude);

    (*position)++;
    if (*position >= half_cycle_samples)
    {
      *position = 0;
    }
  }
}
