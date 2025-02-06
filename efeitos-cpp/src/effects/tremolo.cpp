#include "effects/tremolo.h"
#include <cmath>
#include <vector>
#include <algorithm>

static uint32_t half_cycle_samples;
static uint32_t position;
static float square_state;
static float clickless_sq_state;
static float triangle_state;
static float parabolic_state;
static int waveform;

float clip(float n, float lower, float upper)
{
    return std::max(lower, std::min(n, upper));
}

void tremolo(std::vector<float> &samples, int sampleRate, uint32_t milliseconds)
{
    half_cycle_samples = milliseconds * sampleRate / 1000 / 2;
    position = 0;
    square_state = 1;
    clickless_sq_state = 1;
    triangle_state = -1;
    parabolic_state = 0;
    waveform = 2;

    for (size_t i = 0; i < samples.size(); ++i)
    {
        if (position > half_cycle_samples)
        {
            square_state = -square_state;
            position = 0;
        }

        if (square_state > clickless_sq_state)
        {
            clickless_sq_state += 0.02f;
        }
        else if (square_state < clickless_sq_state)
        {
            clickless_sq_state -= 0.02f;
        }

        clickless_sq_state = clip(clickless_sq_state, -1.0f, 1.0f);

        triangle_state += square_state / (half_cycle_samples / 2.0f);
        triangle_state = clip(triangle_state, -1.0f, 1.0f);

        parabolic_state += triangle_state / (half_cycle_samples / 4.0f);
        parabolic_state = clip(parabolic_state, -1.0f, 1.0f);

        float waveform_state = clickless_sq_state;
        if (waveform == 1)
        {
            waveform_state = triangle_state;
        }
        else if (waveform == 2)
        {
            waveform_state = parabolic_state;
        }

        samples[i] *= (waveform_state + 1) / 2.0f;
        position++;
    }
}
