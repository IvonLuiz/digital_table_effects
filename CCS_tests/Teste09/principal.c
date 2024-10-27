#include "ezdsp5502.h"
#include "ezdsp5502_i2cgpio.h"
#include "csl_gpio.h"
#include "stdio.h"
#include "oled.h"
#include "led.h"

Uint8 estadoSW1 = 0;
Uint8 estadoSW2 = 0;

void checarBotao(void)
{
    Uint8 botaoSW1Pressionado = 0;
    Uint8 botaoSW2Pressionado = 0;

    /* Verifica SW1 */
    if(!(EZDSP5502_I2CGPIO_readLine(SW0))) // SW1 foi pressionado?
    {
        if(estadoSW1) {
            botaoSW1Pressionado = 1; // Marca que SW1 foi pressionado
        }
        estadoSW1 = 0;  // Define estado como pressionado
    } else {
        estadoSW1 = 1;  // Define estado como não pressionado
    }

    /* Verifica SW2 */
    if(!(EZDSP5502_I2CGPIO_readLine(SW1))) // SW2 foi pressionado?
    {
        if(estadoSW2) {
            botaoSW2Pressionado = 1; // Marca que SW2 foi pressionado
        }
        estadoSW2 = 0;  // Define estado como pressionado
    } else {
        estadoSW2 = 1;  // Define estado como não pressionado
    }

    /* Delay para debounce */
    EZDSP5502_waitusec(250000);  // Ajuste o valor conforme necessário (ex.: 50ms)

    /* Ações para botões pressionados */
    if (botaoSW1Pressionado) {
        printf("Botao 01 apertado\n");
        clearPage(1);
        message(0, "  SW1 apertado  ");
        EZDSP5502_I2CGPIO_writeLine(LED2, HIGH);
        EZDSP5502_I2CGPIO_writeLine(LED0, HIGH);
    }

    if (botaoSW2Pressionado) {
        printf("Botao 02 apertado\n");
        clearPage(0);
        message(1, "  SW2 apertado  ");
        EZDSP5502_I2CGPIO_writeLine(LED2, HIGH);
        EZDSP5502_I2CGPIO_writeLine(LED0, HIGH);
    }

    /* Checa se ambos os botões foram pressionados */
    if (botaoSW1Pressionado && botaoSW2Pressionado) {
        printf("Ambos botoes apertados simultaneamente\n");
        clearPage(0);
        clearPage(1);
        message(0, " Ambos pressionados ");
        EZDSP5502_I2CGPIO_writeLine(LED2, HIGH);
        EZDSP5502_I2CGPIO_writeLine(LED0, LOW);
    }
}

void main(void) {

    // Inicializa a placa
    EZDSP5502_init();
    initOLED();
    initLED();

    // Configura o GPIO I2C para os botoes
    EZDSP5502_I2CGPIO_configLine(SW0, IN);
    EZDSP5502_I2CGPIO_configLine(SW1, IN);

    EZDSP5502_I2CGPIO_writeLine(LED2, LOW);

    message(0,"?TESTE BOTAO OLED?");

    while(1)
    {
        checarBotao();
    }
}

