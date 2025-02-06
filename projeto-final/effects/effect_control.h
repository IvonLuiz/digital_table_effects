
#ifndef EFFECTS_PROCESSOR_H
#define EFFECTS_PROCESSOR_H

#include <stdio.h>
#include <stdlib.h>
#include "ezdsp5502.h"
#include "pitch_shifter.h"
#include "reverb.h"
#include "flanger.h"
#include "tremolo.h"

extern int sampleRate;

void apply_effect(FILE *fpIn, FILE *fpOut, int effect, Uint32 *cnt);

#endif // EFFECTS_PROCESSOR_H
