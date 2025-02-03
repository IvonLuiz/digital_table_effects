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

// // Tremolo effect function for Int16 samples
// void apply_tremolo(Int16 *samples, size_t num_samples, int sampleRate, uint32_t milliseconds)
// {
//   uint32_t half_cycle_samples = milliseconds * sampleRate / 1000 / 2;
//   uint32_t position = 0;
//   float square_state = 1.0f;
//   float clickless_sq_state = 1.0f;
//   float triangle_state = -1.0f;
//   float parabolic_state = 0.0f;
//   int waveform = 2;

//   int i = 0;
//   for (; i < num_samples; ++i)
//   {
//     if (position > half_cycle_samples)
//     {
//       square_state = -square_state;
//       position = 0;
//     }

//     // Smooth transition for the square wave
//     if (square_state > clickless_sq_state)
//     {
//       clickless_sq_state += 0.02f;
//     }
//     else if (square_state < clickless_sq_state)
//     {
//       clickless_sq_state -= 0.02f;
//     }

//     // Clamp the clickless state
//     clickless_sq_state = clip(clickless_sq_state, -1.0f, 1.0f);

//     // Generate triangle wave
//     triangle_state += square_state / (half_cycle_samples / 2.0f);
//     triangle_state = clip(triangle_state, -1.0f, 1.0f);

//     // Generate parabolic wave (approximating sine wave)
//     parabolic_state += triangle_state / (half_cycle_samples / 4.0f);
//     parabolic_state = clip(parabolic_state, -1.0f, 1.0f);

//     // Select the active waveform
//     float waveform_state = clickless_sq_state;
//     if (waveform == 1)
//     {
//       waveform_state = triangle_state;
//     }
//     else if (waveform == 2)
//     {
//       waveform_state = parabolic_state;
//     }

//     // Apply tremolo effect to the current sample
//     // float amplitude = (waveform_state + 1.0f) / 2.0f; // Map [-1, 1] to [0, 1]
//     // samples[i] = (Int16)(samples[i] * amplitude);
//     samples[i] *= (waveform_state + 1.0f) / 2.0f;
//     position++;
//   }
// }

void apply_tremolo(int16_t *samples, size_t num_samples, int sampleRate, uint32_t milliseconds, uint32_t *position)
{
  uint32_t half_cycle_samples = (milliseconds * sampleRate) / 1000 / 2; // Half-cycle duration in samples
  float depth = 1.0f;                                                   // Maximum depth of modulation (fade fully to 0)
  int waveform = 1;                                                     // Default: Parabolic waveform (can adjust for testing)
  size_t i;

  for (i = 0; i < num_samples; ++i)
  {
    float waveform_state = 0.0f;  // Initialize the waveform state for the sample
    float triangle_result = 0.0f; // Keep track of triangle wave
    float relative_pos = 0.0f;    // For calculating triangle/parabolic logic

    // Oscillator position handling
    if (*position >= half_cycle_samples)
    {
      *position = 0; // Reset position after one full cycle
    }

    // Generate Triangle Wave (Base)
    relative_pos = (2.0f * (*position) / (float)half_cycle_samples) - 1.0f; // Oscillator [-1.0, 1.0]
    relative_pos = clip(relative_pos, -1.0f, 1.0f);                         // Ensure triangle result stays valid
    triangle_result = relative_pos;                                         // Provide triangle wave as base for parabolic waveform

    // Select waveform type
    if (waveform == 0)
    {
      // Square wave: Simple on/off stutter effect
      waveform_state = (*position < (half_cycle_samples / 2)) ? -1.0f : 1.0f;
    }
    else if (waveform == 1)
    {
      // Triangle wave: Smooth ramp
      waveform_state = triangle_result;
    }
    else if (waveform == 2)
    {
      // Parabolic wave: Derived directly from triangle wave
      waveform_state = 1.0f - (triangle_result * triangle_result); // Smooth parabolic attenuation
    }

    // Map waveform state [-1, 1] to amplitude [0, 1] with depth control
    float amplitude = ((waveform_state + 1.0f) / 2.0f) * depth;

    // Apply tremolo modulation to each sample
    samples[i] = (int16_t)(samples[i] * amplitude);

    // Increment oscillator position
    (*position)++;
    if (*position >= half_cycle_samples)
    {
      *position = 0; // Reset position after one full cycle
    }
  }
}

// void apply_tremolo(int16_t *samples, size_t num_samples, int sampleRate, uint32_t milliseconds, uint32_t *position)
// {
//   uint32_t half_cycle_samples = (milliseconds * sampleRate) / 1000 / 2;
//   float depth = 1.0f; // Maximum depth of modulation
//   int waveform = 2;   // Parabolic waveform as default
//   size_t i = 0;
//   for (; i < num_samples; ++i)
//   {
//     float waveform_state = 0.0f; // Default waveform state

//     if (waveform == 0)
//     { // Square wave
//       waveform_state = (*position < half_cycle_samples / 2) ? -1.0f : 1.0f;
//     }
//     else if (waveform == 1)
//     { // Triangle wave
//       float relative_pos = (2.0f * (*position) / (float)half_cycle_samples) - 1.0f;
//       relative_pos = clip(relative_pos, -1.0f, 1.0f); // Ensure range
//       waveform_state = relative_pos;
//     }
//     else
//     { // Parabolic wave (default)
//       float relative_pos = (2.0f * (*position) / (float)half_cycle_samples) - 1.0f;
//       relative_pos += relative_pos / (half_cycle_samples / 4.0f);
//       relative_pos = clip(relative_pos, -1.0f, 1.0f); // Ensure range

//       if (relative_pos <= -1.0f || relative_pos >= 1.0f)
//       {
//         waveform_state = 0.0f; // Force edges to zero
//       }
//       else
//       {
//         waveform_state = 1.0f - (relative_pos * relative_pos);
//       }

//       // Map from [-1, 1] to [0, 1] for amplitude
//       float amplitude = ((waveform_state + 1.0f) / 2.0f) * depth;

//       // Apply modulation to audio sample
//       samples[i] = (int16_t)(samples[i] * amplitude);

//       // Advance oscillator position
//       (*position)++;
//       if (*position >= half_cycle_samples)
//       {
//         *position = 0; // Reset position after one full cycle
//       }
//     }
//   }
