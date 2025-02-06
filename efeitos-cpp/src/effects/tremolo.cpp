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

float clip(float n, float lower, float upper) {
  return std::max(lower, std::min(n, upper));
}

void tremolo(std::vector<float>& samples, int sampleRate, uint32_t milliseconds) {
    half_cycle_samples = milliseconds * sampleRate / 1000 / 2;
    position = 0;
    square_state = 1;
    clickless_sq_state = 1;
    triangle_state = -1;
    parabolic_state = 0;
    waveform = 2;
    
    for (size_t i = 0; i < samples.size(); ++i) {
        if (position > half_cycle_samples) {
            square_state = -square_state;
            position = 0;
        }

        // An actual square wave will make clicking sounds at the
	    // edges when the audio suddenly cuts in and out. To avoid
	    // that, we do a quick fade-in and fade-out instead of a
	    // sudden cut.
        if (square_state > clickless_sq_state) {
            clickless_sq_state += 0.02f;
        } else if (square_state < clickless_sq_state) {
            clickless_sq_state -= 0.02f;
        }

        // Clamp the output value between -1 and 1, because float
        // point, discrete math isn't precise and we don't want any
        // little offset from the floats or sampling to add up over
        // time.
        clickless_sq_state = clip(clickless_sq_state, -1.0f, 1.0f);

        // To get a triangle wave, we just add up the square samples,
        // but scale them down so that the positive cycle samples sum
        // to 2, taking our triangle wave from -1 to 1, and subtract 2
        // during the negative to take us back to -1.
        triangle_state += square_state / (half_cycle_samples / 2.0f);
        triangle_state = clip(triangle_state, -1.0f, 1.0f);

        // To get something more like a sine wave, we can add up the
        // triangle wave samples, again scaling so the wave ranges
        // from -1 to 1 (each cycle of the triangle wave has half the
        // area of a square, so we divide by 2 again). This produces a
        // parabolic wave, which is close enough to a sine wave for
        // our purposes.
        parabolic_state += triangle_state / (half_cycle_samples / 4.0f);
        parabolic_state = clip(parabolic_state, -1.0f, 1.0f);
        
        // Shift the final wave form into the range from 0 to 1, then
        // multiply by the audio sample to scale its volume.
        float waveform_state = clickless_sq_state;
        if (waveform == 1) {
            waveform_state = triangle_state;
        } else if (waveform == 2) {
            waveform_state = parabolic_state;
        }

        samples[i] *= (waveform_state + 1) / 2.0f;
        position++;
    }
}
