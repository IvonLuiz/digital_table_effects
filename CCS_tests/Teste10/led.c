/*
 * led.c
 *
 *  Created on: 26 de out de 2024
 *      Author: Heverton Reis
 */
#include "ezdsp5502.h"
#include "ezdsp5502_i2cgpio.h"

void initLED(void)
{
    /* Inicializar LEDs */
    /* Configurar GPIO I2C para os LEDs*/
    EZDSP5502_I2CGPIO_configLine(LED0, OUT);
    EZDSP5502_I2CGPIO_configLine(LED1, OUT);
    EZDSP5502_I2CGPIO_configLine(LED2, OUT);
    EZDSP5502_I2CGPIO_configLine(LED3, OUT);

    /* Desliga todos os LEDs por linha*/
    EZDSP5502_I2CGPIO_writeLine(LED0, HIGH);
    EZDSP5502_I2CGPIO_writeLine(LED1, HIGH);
    EZDSP5502_I2CGPIO_writeLine(LED2, HIGH);
    EZDSP5502_I2CGPIO_writeLine(LED3, HIGH);
}
