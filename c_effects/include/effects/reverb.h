#pragma once
#include <math.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

enum Effect
{
  REV_HALL1,
  REV_ROOM2_,
  REV_STAGE_B,
  REV_STAGE_D,
  REV_STAGE_F,
  REV_STAGE_Gb,
};

#define PI 3.141592653589793

void apply_reverb(int16_t *data, uint32_t numSamples, int delay, float decay);

float band_pass_filter(float sample, float centerFreq, float q,
                       float sampleRate, float *state);

void reverse_samples(int16_t *data, uint32_t numSamples);

void apply_rev_hall(int16_t *data, uint32_t numSamples, uint32_t sampleRate);