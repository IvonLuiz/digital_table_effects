#ifndef FLANGER_H
#define FLANGER_H

#include <stdint.h>
#include <stddef.h>
#include "tistdtypes.h"

#define M_PI 3.14159265358979323846

typedef struct
{
    float phase;
    int writeIndex;
    Int16 *delayBuffer;
} FlangerState;

void apply_flanger(int16_t *samples, size_t numSamples, int sampleRate, float delayMs, float depthMs, float rateHz);

#endif // FLANGER_H
