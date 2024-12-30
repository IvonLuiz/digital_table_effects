#ifndef COMB_FILTER_H
#define COMB_FILTER_H

#include <vector>
#include <cstdint>

// std::vector<int16_t> combFilter(std::vector<int16_t> inputSamples, double delayMs, double decayGain, float sampleRate);

std::vector<int16_t> combFilter(const std::vector<int16_t>& inputSamples, double delayMs, double decayGain, float sampleRate);
std::vector<int16_t> allPassFilter(const std::vector<int16_t>& inputSamples, double delayMs, double decayGain, float sampleRate);
std::vector<int16_t> schroederReverb(const std::vector<int16_t>& inputSamples, float sampleRate);
std::vector<int16_t> mixDryWet(const std::vector<int16_t>& dry, const std::vector<int16_t>& wet, float wetLevel);

#endif // COMB_FILTER_H
