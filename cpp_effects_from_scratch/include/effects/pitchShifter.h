#ifndef PITCHSHIFTER_H
#define PITCHSHIFTER_H

#include <vector>
#include <complex>

using Sample = float;

// Function to generate the adjusted sawtooth wave
float sawtooth(float phase, float amplitude);

// Envelope calculation function (fade-in and fade-out to smooth transitions)
float envelope(float phase);

// Pitch shifting using two delay lines
void shiftPitch(const std::vector<float>& input, std::vector<float>& output, int sampleRate, float pitchShift);
 

#endif // PITCHSHIFTER_H