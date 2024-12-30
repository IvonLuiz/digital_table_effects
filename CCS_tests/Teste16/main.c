#include <stdio.h>
#include "ezdsp5502.h"
#include "ezdsp5502_i2cgpio.h"
#include "csl_gpio.h"

#include "dma.h"

extern void AIC3204_init(void);
extern void configAudioDma(void);
extern void startAudioDma(void);
extern void stopAudioDma(void);
extern void initPLL(void);
extern void dataMove(Int16 *, Int16 *);

Int16 dsp_process(Int16 *input, Int16 *output, Int16 size);

#define BUFFER_SIZE 512

Int16 RcvBuffer1[BUFFER_SIZE];
Int16 XmitBuffer1[BUFFER_SIZE];

void main(void)
{
    Int16 status, i;

    for (i = 0; i < BUFFER_SIZE; i++) {                  // Garante que os buffers estejam limpos antes de qualquer operação
        RcvBuffer1[i] = 0;
        XmitBuffer1[i] = 0;
    }

    initPLL();
    EZDSP5502_init();       // Inicializa os recursos basicos da placa eZdsp5502.

    AIC3204_init();         // Configura o codec de audio AIC3204 para captura e reproducao de audio.

    configAudioDma();       // Configura os canais DMA para transmissao e recepcao de audio.
    startAudioDma();

    status = 1;
    while(status){
        status = dsp_process(RcvBuffer1,XmitBuffer1,BUFFER_SIZE);
    }
}

Int16 dsp_process(Int16 *input, Int16 *output, Int16 size) {
    dataMove(input,output);
    return 1;
}

