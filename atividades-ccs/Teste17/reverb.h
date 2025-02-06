#include "ezdsp5502.h"

#define SAMPLE_RATE 48000  // Taxa de amostragem (48 kHz)
#define BUFFER_SIZE 512    // Tamanho dos buffers
#define MAX_DELAY 1024     // Máximo atraso para os filtros (em amostras)

// Buffer circular para Comb Filters
static Int16 delayBuffer[MAX_DELAY];
static int delayIndex = 0;

// Comb Filter
void combFilter(Int16 *input, Int16 *output, int length, float delayMs, float decayFactor);

// All-Pass Filter
void allPassFilter(Int16 *input, Int16 *output, int bufferSize, float delayMs);
