
#ifndef EFFECTS_PROCESSOR_H
#define EFFECTS_PROCESSOR_H

#include <stdio.h>
#include <stdlib.h>    // For standard utilities
#include "ezdsp5502.h" // For Uint8, Uint32, Int8, Int16, etc.
#include "pitch_shifter.h"
#include "reverb.h"
#include "flanger.h"
#include "tremolo.h"

// Constants
extern int sampleRate; // Sample rate (default: 8000)

void apply_effect(FILE *fpIn, FILE *fpOut, int effect, Uint32 *cnt);

#endif // EFFECTS_PROCESSOR_H
