/*
 * i2cgpio.c
 *
 *  Created on: 13 de out de 2024
 *      Author: Heverton Reis
 */
#include "i2cgpio.h"
#include "ezdsp5502_i2cgpio.h"

void initSWAndLed(void){

    /* Configuracao dos GPIOs para os Botoes */
    EZDSP5502_I2CGPIO_configLine(SW0,IN); // Configura o pino associado ao botao SW0 como uma entrada (IN).
    EZDSP5502_I2CGPIO_configLine(SW1,IN); // Configura o pino associado ao botao SW1 como uma entrada (IN).

    /* Configuracao dos GPIOs para os LEDs
        Configura o pino associado aos LEDs LED0, LED1, LED2 e LED3 como uma saida (OUT).
    */
    EZDSP5502_I2CGPIO_configLine(LED0,OUT);
    EZDSP5502_I2CGPIO_configLine(LED1,OUT);
    EZDSP5502_I2CGPIO_configLine(LED2,OUT);
    EZDSP5502_I2CGPIO_configLine(LED3,OUT);

    /* Desliga Todos os LEDs */
    EZDSP5502_I2CGPIO_writeLine(LED0,HIGH);
    EZDSP5502_I2CGPIO_writeLine(LED1,HIGH);
    EZDSP5502_I2CGPIO_writeLine(LED2,HIGH);
    EZDSP5502_I2CGPIO_writeLine(LED3,HIGH);
}
