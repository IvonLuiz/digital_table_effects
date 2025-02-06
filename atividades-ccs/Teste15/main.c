#include <stdio.h>
#include "ezdsp5502.h"
#include "ezdsp5502_i2cgpio.h"
#include "csl_gpio.h"

#include "dma.h"

#define SF48KHz         48000
#define SF24KHz         24000
#define SF16KHz         16000
#define SF12KHz         12000
#define SF8KHz          8000

#define DAC_GAIN        3       // 3dB range: -6dB to 29dB
#define ADC_GAIN        0       // 0dB range: 0dB to 46dB

#define BUFFER_SIZE 256

Int16 RcvBuffer1[BUFFER_SIZE];
Int16 RcvBuffer2[BUFFER_SIZE];
Int16 XmitBuffer1[BUFFER_SIZE];
Int16 XmitBuffer2[BUFFER_SIZE];

extern void AIC3204_init(void);
extern void configAudioDma(void);
extern void startAudioDma(void);
extern void stopAudioDma(void);
extern void initPLL(void);

Int16 dsp_process(Int16 *input, Int16 *output, Int16 size);

void main(void)
{
    Int16 status, i;

    for (i = 0; i < BUFFER_SIZE; i++) {                  // Garante que os buffers estejam limpos antes de qualquer operacao de transmissao ou recepcao.
        RcvBuffer1[i] = 0;
        RcvBuffer2[i] = 0;
        XmitBuffer1[i] = 0;
        XmitBuffer2[i] = 0;
    }

    initPLL();
    EZDSP5502_init();       // Inicializa os recursos basicos da placa eZdsp5502.

    AIC3204_init();

    configAudioDma();       // Configura os canais DMA para transmissao e recepcao de audio.
    startAudioDma();

    status = 1;
    while(status){
        if(currentRxBuffer == 1){
            status = dsp_process(RcvBuffer1,XmitBuffer1,BUFFER_SIZE);
        }
        else{
            status = dsp_process(RcvBuffer2,XmitBuffer2,BUFFER_SIZE);
        }
    }
}

Int16 dsp_process(Int16 *input, Int16 *output, Int16 size) {

    Int16 i;
    for(i=0; i<size; i++)
    {
        *(output + i) = *(input + i);
    }

    return 1;
}
