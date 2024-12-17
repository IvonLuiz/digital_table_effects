#include <stdio.h>
#include "ezdsp5502.h"
#include "ezdsp5502_i2cgpio.h"
#include "csl_gpio.h"
#include "reverb.h"

#include "dma.h"

extern void AIC3204_init(void);
extern void configAudioDma(void);
extern void startAudioDma(void);
extern void stopAudioDma(void);
extern void initPLL(void);

#define BUFFER_SIZE 512

Int16 RcvBuffer1[BUFFER_SIZE];
Int16 XmitBuffer1[BUFFER_SIZE];
Int16 processedBuffer[BUFFER_SIZE];
Int16 combFilterSamples1[BUFFER_SIZE];
Int16 combFilterSamples2[BUFFER_SIZE];
Int16 combFilterSamples3[BUFFER_SIZE];
Int16 combFilterSamples4[BUFFER_SIZE];

void main(void)
{
    Int16 status, i;

    for (i = 0; i < BUFFER_SIZE; i++) {                  // Garante que os buffers estejam limpos antes de qualquer opera��o
        RcvBuffer1[i] = 0;
        XmitBuffer1[i] = 0;
        processedBuffer[i] = 0;
        combFilterSamples1[i] = 0;
        combFilterSamples2[i] = 0;
        combFilterSamples3[i] = 0;
        combFilterSamples4[i] = 0;
    }

    initPLL();
    EZDSP5502_init();       // Inicializa os recursos basicos da placa eZdsp5502.

    AIC3204_init();         // Configura o codec de audio AIC3204 para captura e reproducao de audio.

    configAudioDma();       // Configura os canais DMA para transmissao e recepcao de audio.
    startAudioDma();


    float delayMs = 50;      // Atraso inicial (50 ms)
    float decayFactor = 0.7;   // Fator de decaimento
    float mix = 0.5f;           // Percentual de mistura (50%)


    status = 1;
    while (status) {
         // Aplica Comb Filters e mistura
         combFilter(RcvBuffer1, combFilterSamples1, BUFFER_SIZE, delayMs, decayFactor);
         combFilter(RcvBuffer1, combFilterSamples2, BUFFER_SIZE, delayMs-11.73f, decayFactor- 0.1313f);
         combFilter(RcvBuffer1, combFilterSamples3, BUFFER_SIZE, delayMs+19.31f, decayFactor-0.2743f);
         combFilter(RcvBuffer1, combFilterSamples4, BUFFER_SIZE, delayMs-7.97f, decayFactor-0.31f);

         for (i = 0; i < BUFFER_SIZE; i++) {
             //processedBuffer[i] = (Int16)((1.0f - mix) * RcvBuffer1[i] + mix * processedBuffer[i]);
             processedBuffer[i] = ((combFilterSamples1[i] + combFilterSamples2[i] + combFilterSamples3[i] + combFilterSamples4[i])) ;
         }

         for (i = 0; i < BUFFER_SIZE; i++) {
             processedBuffer[i] = (Int16)((1.0f - mix) * RcvBuffer1[i] + mix * processedBuffer[i]);
         }

         // Aplica All-Pass Filters
         allPassFilter(processedBuffer, processedBuffer, BUFFER_SIZE, 10);
         allPassFilter(processedBuffer, processedBuffer, BUFFER_SIZE, 10);

         // Copia para o buffer de transmiss�o
         for (i = 0; i < BUFFER_SIZE; i++) {
             XmitBuffer1[i] = processedBuffer[i];
         }
    }
}

