#include "tremolo.h"
#include <math.h>
#include <stdint.h>
#include <stdlib.h>

float fminf(float a, float b)
{
  return (a < b) ? a : b;
}

// Implementao de fmaxf (retorna o maior dos dois nmeros)
float fmaxf(float a, float b)
{
  return (a > b) ? a : b;
}

// Function to clip values between a lower and upper bound
static float clip(float n, float lower, float upper)
{
  return fmaxf(lower, fminf(n, upper));
}

// Tremolo effect function for int16_t samples
void apply_tremolo(int16_t *samples, size_t num_samples, int sampleRate, uint32_t milliseconds)
{
  uint32_t half_cycle_samples = milliseconds * sampleRate / 1000 / 2;
  uint32_t position = 0;
  float square_state = 1.0f;
  float clickless_sq_state = 1.0f;
  float triangle_state = -1.0f;
  float parabolic_state = 0.0f;
  int waveform = 2;

  size_t i = 0;
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
    float amplitude = (waveform_state + 1.0f) / 2.0f; // Map [-1, 1] to [0, 1]
    samples[i] = (int16_t)(samples[i] * amplitude);

    position++;
  }
}
void apply_tremolo(int16_t *samples, size_t num_samples, int sampleRate, uint32_t milliseconds, uint32_t *position)
{
  uint32_t half_cycle_samples = milliseconds * sampleRate / 1000 / 2;

  float square_state = 1.0f;
  float clickless_sq_state = 1.0f;
  float triangle_state = -1.0f;
  float parabolic_state = 0.0f;
  int waveform = 2;

  // Process each sample in the buffer
  for (size_t i = 0; i < num_samples; ++i)
  {
    if (*position > half_cycle_samples)
    {
      square_state = -square_state;
      *position = 0; // Reset position at the end of the half-cycle
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
    float amplitude = (waveform_state + 1.0f) / 2.0f; // Map [-1, 1] to [0, 1]
    samples[i] = (int16_t)(samples[i] * amplitude);

    (*position)++; // Advance the oscillator position
  }
}
