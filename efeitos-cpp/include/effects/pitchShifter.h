#ifndef PITCHSHIFTER_H
#define PITCHSHIFTER_H

#include <vector>
#include <complex>

using Sample = float;

float sawtooth(float phase, float amplitude);

float envelope(float phase);

void shiftPitch(const std::vector<float> &input, std::vector<float> &output, int sampleRate, float pitchShift);

#endif