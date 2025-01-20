#ifndef FLANGER_H
#define FLANGER_H

#include <vector>

void flanger(std::vector<float>& samples, int sampleRate, float delayMs, float depthMs, float rateHz);

#endif // FLANGER_H