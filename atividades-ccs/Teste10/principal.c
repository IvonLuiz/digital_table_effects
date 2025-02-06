#include "ezdsp5502.h"
#include "ezdsp5502_i2cgpio.h"
#include "csl_gpio.h"
#include "stdio.h"
#include "oled.h"
#include "aic3204.h"

void loopCapture(void)
{
    Int16 sample, dataLeft, dataRight;

    dataLeft = 0;
    dataRight = 0;

    while (1)                                     // Checa o estado de captura
    {
        for (sample = 0; sample < 48; sample++)   // 48 amostras por milissegundo
        {
            EZDSP5502_MCBSP_read(&dataLeft);      // Leitura do canal esquerdo
            EZDSP5502_MCBSP_write(dataLeft);      // Envio de volta para o canal esquerdo

            EZDSP5502_MCBSP_read(&dataRight);     // Leitura do canal direito
            EZDSP5502_MCBSP_write(dataRight);     // Envio de volta para o canal direito
        }

    }
}

void main(void)
{
    // Inicializa a placa
    EZDSP5502_init();
    initOLED();
    initAIC3204();

    EZDSP5502_I2CGPIO_writeLine(LED2, LOW);

    message(0, "TESTE CAPTURA AUDIO");

    while (1)
    {
        message(1, " MODO CAPTURA ");
        loopCapture();
    }

}
