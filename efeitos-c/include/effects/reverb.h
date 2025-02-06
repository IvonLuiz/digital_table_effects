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

float band_pass_filter(float sample, float centerFreq, float q,
                       float sampleRate, float *state);

void all_pass_filter(int16_t *samples, size_t numSamples, float delayMs, float decayGain, int sampleRate);

void comb_filter(int16_t *samples, size_t numSamples, float delayMs, float decayGain, int sampleRate);

void reverse_samples(int16_t *data, uint32_t numSamples);

void apply_reverb_simple(int16_t *data, uint32_t numSamples, uint32_t sampleRate, float delay, float decay);

void apply_reverb_hall(int16_t *data, uint32_t numSamples, uint32_t sampleRate, float delay, float decay);

void apply_reverb_shroeder(int16_t *samples, size_t numSamples, int sampleRate);

void mix_dry_wet(int16_t *dry, int16_t *wet, float *output, size_t numSamples, float wetLevel);
