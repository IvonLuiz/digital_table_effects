#ifndef COMB_FILTER_H
#define COMB_FILTER_H

#include <vector>
#include <cstdint>

std::vector<int16_t> combFilter(std::vector<int16_t> inputSamples, double delayMs, double decayGain, float sampleRate);

#endif // COMB_FILTER_H
