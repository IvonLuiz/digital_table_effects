#ifndef TREMOLO_H
#define TREMOLO_H

#include <stdint.h>
#include <stddef.h>

// Declaracao da funcao apply_tremolo
// void apply_tremolo(Int16 *samples, size_t num_samples, int sampleRate, uint32_t milliseconds);
void apply_tremolo(int16_t *samples, size_t num_samples, int sampleRate, uint32_t milliseconds, uint32_t *position);

#endif // TREMOLO_H
