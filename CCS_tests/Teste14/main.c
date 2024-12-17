#include <stdio.h>
#include "ezdsp5502.h"
#include "ezdsp5502_i2cgpio.h"
#include "csl_gpio.h"

extern void AIC3204_init(void);
extern void configAudioDma(void);
extern void startAudioDma(void);
extern void stopAudioDma(void);


void main(void)
{

    EZDSP5502_init();       // Inicializa os recursos basicos da placa eZdsp5502.

    AIC3204_init();         // Configura o codec de audio AIC3204 para captura e reproducao de audio.

    configAudioDma();       // Configura os canais DMA para transmissao e recepcao de audio.

    while(1){

        startAudioDma();

    }

}
