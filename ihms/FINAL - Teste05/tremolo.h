#pragma once
#ifndef TREMOLO_H
#define TREMOLO_H

#include <stddef.h> // For size_t
#include <stdint.h> // For uint32_t

// Function to apply a tremolo effect to audio samples
// Parameters:
// - samples: Pointer to the array of audio samples (in int16_t format, values between -32768 and 32767)
// - num_samples: Number of samples in the array
// - sampleRate: Audio sample rate (e.g., 44100 for CD-quality audio)
// - milliseconds: Duration of the tremolo half-cycle in milliseconds
void apply_tremolo(int16_t *samples, size_t num_samples, int sampleRate, uint32_t milliseconds, uint32_t *position);

#endif // TREMOLO_H
