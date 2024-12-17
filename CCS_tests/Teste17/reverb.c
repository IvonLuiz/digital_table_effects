#include <math.h>
#include <string.h>
#include <reverb.h>

// Comb Filter
void combFilter(Int16 *input, Int16 *output, int length, float delayMs, float decayFactor) {
    int delaySamples = (int)((delayMs * SAMPLE_RATE) / 1000.0f);  // Atraso em amostras
    int i;
    for (i = 0; i < length; i++) {
        int delayedIdx = (delayIndex + MAX_DELAY - delaySamples) % MAX_DELAY;
        float delayedSample = delayBuffer[delayedIdx];

        // Atualiza o buffer de atraso
        delayBuffer[delayIndex] = input[i] + (delayedSample * decayFactor);
        delayIndex = (delayIndex + 1) % MAX_DELAY;

        // Sa�da
        output[i] = delayBuffer[delayIndex];
    }
}

// All-Pass Filter
void allPassFilter(Int16 *input, Int16 *output, int length, float delayMs) {
    int delaySamples = (int)((delayMs * SAMPLE_RATE) / 1000.0f);  // Atraso em amostras
    static Int16 apDelayBuffer[MAX_DELAY];
    static int apDelayIndex = 0;
    float decayFactor = 0.131f;

    int i;
    for (i = 0; i < length; i++) {
        int delayedIdx = (apDelayIndex + MAX_DELAY - delaySamples) % MAX_DELAY;
        float delayedSample = apDelayBuffer[delayedIdx];

        // All-pass formula
        float temp = input[i] + (delayedSample * decayFactor);
        output[i] = delayedSample - (temp * decayFactor);

        // Atualiza o buffer de atraso
        apDelayBuffer[apDelayIndex] = temp;
        apDelayIndex = (apDelayIndex + 1) % MAX_DELAY;
    }
}

// void allPassFilter(Int16 *input, Int16 *output, int bufferSize, float delayMs)
// {
//     int delaySamples = (int)(delayMs * (SAMPLE_RATE / 1000));
//     float decayFactor = 0.131f;
//     int i;

//     for (i = 0; i < bufferSize; i++) {
//         output[i] = input[i];

//         if (i - delaySamples >= 0) {
//             output[i] += -decayFactor * output[i - delaySamples];
//         }

//         if (i - delaySamples >= 1) {
//             output[i] += decayFactor * output[i + 20 - delaySamples];
//         }
//     }

//     // Normaliza os samples para evitar clipping
//     float max = 0.0f;
//     for (i = 0; i < bufferSize; i++) {
//         if (fabs(output[i]) > max) {
//             max = fabs(output[i]);
//         }
//     }

//     for (i = 0; i < bufferSize; i++) {
//         output[i] /= max;
//     }
// }


// void allPassFilter(Int16 *input, Int16 *output, int length, float delayMs) {
//     int delaySamples = (int)((delayMs * SAMPLE_RATE) / 1000.0f);  // Atraso em amostras
//     static float apDelayBuffer[MAX_DELAY];  // Buffer para armazenar amostras atrasadas
//     static int apDelayIndex = 0;  // �
//     float decayFactor = 0.131f;   // Fator de decaimento

//     float max = 0.0f;  // Para normali
//     float currentValue, smoothedValue = 0.0f;

//     // Aplicdo filtro All-Pass

//     int i;
//     for (i = 0; i < length; i++) {
//         int delayedIdx = (apDelayIndex + MAX_DELAY - delaySamples) % MAX_DELAY;  //ce atrasado
//         float delayedSample = apDelayBuffer[delayedIdx];

//         //  do filtro All-Pass
//         float temp = input[i] + (-decayFactor * delayedSample);  // Entrada com atraso negativo
//         apDelayBuffer[apDelayIndex] = temp;  // Atualiza o buffer de atraso
//         output[i] = delayedSample + (decayFactor * temp);  // Calcula a

//         // Atualiza o índice do buffer circular
//         apDelayIndex = (apDelayIndex + 1) % MAX_DELAY;
//     }

//     // Normaliz do dio para evitar clipping
//     for (i = 0; i < length; i++) {
//         if (fabs(output[i]) > max) {
//             max = fabs(output[i]);
//         }
//     }

//     if (max > 0.0f) {
//         for (i = 0; i < length; i++) {
//             currentValue = output[i];
//             smoothedValue = smoothedValue + ((currentValue - smoothedValue) / max);
//             output[i] = smoothedValue;  // Ajusta os valores suavizados
//         }
//     }
// }
