#ifndef COMB_FILTER_H
#define COMB_FILTER_H

#include <vector>

std::vector<float> combFilter(const std::vector<float> &inputSamples, double delayMs, double decayGain, float sampleRate);
std::vector<float> allPassFilter(const std::vector<float> &inputSamples, double delayMs, double decayGain, float sampleRate);
std::vector<float> schroederReverb(const std::vector<float> &inputSamples, float sampleRate);
std::vector<float> mixDryWet(const std::vector<float> &dry, const std::vector<float> &wet, float wetLevel);

#endif // COMB_FILTER_H
