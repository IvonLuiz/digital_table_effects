/*
 * principal.c
 *
 *  Created on: 13 de out de 2024
 *      Author: Heverton Reis
 */
#include "ezdsp5502.h"
#include "ezdsp5502_mcbsp.h"
#include "ezdsp5502_i2cgpio.h"
#include "csl_chip.h"

#include "pll.h"
#include "i2cgpio.h"
#include "timer.h"
#include "aic3204.h"
#include "dma.h"
#include "stdio.h"

void configPort(void);
void checkTimer(void);
void checkSwitch(void);

extern Int16 oled_start();
extern Uint16 timerFlag;  // Timer interrupt flag

Uint8 ledNum = 3;         // I2C GPIO numero do LED
Uint8 sw1State = 0;       // Estado de SW1
Uint8 sw2State = 0;       // Estado de SW2

void main(void){

    /* Configuracoes iniciais */
    printf("Realizando Configuracoes iniciais\n");
    initPLL();         // Inicializa o PLL
    EZDSP5502_init( ); // Inicializa a placa
    initSWAndLed();    // Inicializa o I2C GPIO para os LEDs e Botoes
    configPort();      // Configura os MUXs
    initTimer0();      // Inicializa o Timer0
    initAIC3204();     // Inicializa o AIC3204 - codec de audio
    configAudioDma();  // Configura o DMA com o tom de audio

    /* Inicializacao da demonstracao */
    printf("Configurando Demonstracoes\n");
    startAudioDma();            // Inicia o DMA para o servico McBSP
    EZDSP5502_MCBSP_init( );    // Configura e inicia o McBSP
    startTimer0();              // Inicia o timer
    oled_start();               // Configura e inicia o display

    fflush(stdout);
    while(1){
        checkTimer();  // Verifica se a interrupcao do timer foi acionada
        checkSwitch(); // Verifica se o botão foi pressionado
    }
}

static toggleLED(void)
{
    if(CHIP_FGET(ST1_55, XF))
        CHIP_FSET(ST1_55, XF,CHIP_ST1_55_XF_OFF);  // Desliga o LED
    else
        CHIP_FSET(ST1_55, XF, CHIP_ST1_55_XF_ON);  // Liga o LED
}

void configPort(void)
{
    /* Configura o GPIO BSP_SEL1 para o modo McBSP1
     * (Saída, Low = Seleciona o McBSP1) */
    EZDSP5502_I2CGPIO_configLine( BSP_SEL1, OUT );
    EZDSP5502_I2CGPIO_writeLine(  BSP_SEL1, LOW );

    /* Habilita o McBSP1 (Saída, Low = Habilita o McBSP1) */
    EZDSP5502_I2CGPIO_configLine( BSP_SEL1_ENn, OUT );
    EZDSP5502_I2CGPIO_writeLine(  BSP_SEL1_ENn, LOW );
}

void checkTimer(void)
{
    if(timerFlag == 1)   // Verifica se o timerFlag esta definido como 1
    {
        timerFlag = 0;    // Reseta o timerFlag para 0 para indicar que a acao foi realizada
        toggleLED();      // Chama a funcao toggleLED para alternar o estado de um LED
        EZDSP5502_I2CGPIO_writeLine( ledNum,   // Escreve no GPIO correspondente ao LED
                 (~EZDSP5502_I2CGPIO_readLine(ledNum) & 0x01) ); // Alterna o estado do LED

        if( ledNum > 6)   // Verifica se ledNum e maior que 6
            ledNum = 3;   // Se for, redefine ledNum para 3 (volta para o primeiro LED)

        ledNum++;         // Incrementa ledNum para passar para o proximo LED
    }
}

void checkSwitch(void)
{
    /* Verifica SW1 */
    if(!(EZDSP5502_I2CGPIO_readLine(SW0))) // O SW1 esta pressionado?
    {
        if(sw1State)          // O estado anterior era "nao pressionado"?
        {
            printf("Botao01 apertado");
            changeTimer();    // Muda o periodo do temporizador
            sw1State = 0;     // Define o estado para 0 para permitir apenas uma unica pressionada
        }
    }
    else                      // SW1 não esta pressionado
        sw1State = 1;         // Define o estado para 1 para permitir a mudanca do temporizador

    /* Verifica SW2 */
    if(!(EZDSP5502_I2CGPIO_readLine(SW1))) // O SW2 esta pressionado?
    {
        if(sw2State)          // O estado anterior era "nao pressionado"?
        {
            printf("Botao02 apertado");
            changeTone();     // Muda o tom do audio
            sw2State = 0;     // Define o estado para 0 para permitir apenas uma unica pressionada
        }
    }
    else                      // SW2 não esta pressionado
        sw2State = 1;         // Define o estado para 1 para permitir a mudanca do tom
}

