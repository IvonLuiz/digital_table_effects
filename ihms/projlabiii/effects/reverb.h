#ifndef REVERB_H
#define REVERB_H

#include <stdint.h>
#include <stddef.h>
#include "tistdtypes.h"

enum Effect
{
  REV_HALL1,
  REV_ROOM2_,
  REV_STAGE_B,
  REV_STAGE_D,
  REV_STAGE_F,
  REV_STAGE_Gb
};

#define PI 3.141592653589793

void apply_reverb_simple(Int16 *data, uint32_t numSamples, uint32_t sampleRate, float delay, float decay);
void apply_reverb_hall(Int16 *data, uint32_t numSamples, uint32_t sampleRate, float delay, float decay);
void apply_reverb_shroeder(Int16 *samples, size_t numSamples, int sampleRate);

#endif // REVERB_H
