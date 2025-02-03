#include <math.h>
#include <stdint.h>
#include <stdlib.h>
#include "tistdtypes.h"

// Implementacao manual de fmaxf e fminf
static float fmaxf(float a, float b)
{
  return (a > b) ? a : b;
}

static float fminf(float a, float b)
{
  return (a < b) ? a : b;
}

// Function to clip values between a lower and upper bound
static float clip(float n, float lower, float upper)
{
  return fmaxf(lower, fminf(n, upper));
}

// Tremolo effect function for Int16 samples
void apply_tremolo(Int16 *samples, size_t num_samples, int sampleRate, uint32_t milliseconds)
{
  uint32_t half_cycle_samples = milliseconds * sampleRate / 1000 / 2;
  uint32_t position = 0;
  float square_state = 1.0f;
  float clickless_sq_state = 1.0f;
  float triangle_state = -1.0f;
  float parabolic_state = 0.0f;
  int waveform = 2;

  int i = 0;
  for (; i < num_samples; ++i)
  {
    if (position > half_cycle_samples)
    {
      square_state = -square_state;
      position = 0;
    }

    // Smooth transition for the square wave
    if (square_state > clickless_sq_state)
    {
      clickless_sq_state += 0.02f;
    }
    else if (square_state < clickless_sq_state)
    {
      clickless_sq_state -= 0.02f;
    }

    // Clamp the clickless state
    clickless_sq_state = clip(clickless_sq_state, -1.0f, 1.0f);

    // Generate triangle wave
    triangle_state += square_state / (half_cycle_samples / 2.0f);
    triangle_state = clip(triangle_state, -1.0f, 1.0f);

    // Generate parabolic wave (approximating sine wave)
    parabolic_state += triangle_state / (half_cycle_samples / 4.0f);
    parabolic_state = clip(parabolic_state, -1.0f, 1.0f);

    // Select the active waveform
    float waveform_state = clickless_sq_state;
    if (waveform == 1)
    {
      waveform_state = triangle_state;
    }
    else if (waveform == 2)
    {
      waveform_state = parabolic_state;
    }

    // Apply tremolo effect to the current sample
    // float amplitude = (waveform_state + 1.0f) / 2.0f; // Map [-1, 1] to [0, 1]
    // samples[i] = (Int16)(samples[i] * amplitude);
    samples[i] *= (waveform_state + 1.0f) / 2.0f;
    position++;
  }
}
