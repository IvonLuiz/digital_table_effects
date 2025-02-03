#ifndef FLANGER_H
#define FLANGER_H

#include <stdint.h>
#include <stddef.h>
#include "tistdtypes.h"

#define M_PI 3.14159265358979323846

// Estrutura para armazenar o estado do flanger
typedef struct
{
    float phase;
    int writeIndex;
    Int16 *delayBuffer;
} FlangerState;

// Declaração da função apply_flanger
void apply_flanger(Int16 *samples, size_t numSamples, int sampleRate, float delayMs, float depthMs, float rateHz);

#endif // FLANGER_H
