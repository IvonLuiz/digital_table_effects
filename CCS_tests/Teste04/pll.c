/*
 * pll.c
 *
 *  Created on: 13 de out de 2024
 *      Author: Heverton Reis
 */
#include "pll.h"
#include "ezdsp5502.h"
#include "csl.h"
#include "csl_pll.h"

PLL_Config myConfig = {
    /* Configura o registro de controle e status do PLL */
    PLL_PLLCSR_RMK(
        PLL_PLLCSR_PLLRST_RESET_RELEASED, // Libera o reset do PLL.
        PLL_PLLCSR_OSCPWRDN_OSC_ON, // Liga o oscilador (mantem o oscilador ativo).
        PLL_PLLCSR_PLLPWRDN_PLL_ON, // Mantem o PLL ligado.
        PLL_PLLCSR_PLLEN_DEFAULT // Usa o valor padrao para habilitar o PLL.
    ),
    /* Configuracao do Multiplicador do PLL */
    PLL_PLLM_PLLM_OF(10), // Saida @ Entrada x10.
    /* Configuracao do Divisor 0 */
    PLL_PLLDIV0_RMK(
        PLL_PLLDIV0_D0EN_ENABLED, // Ativa o divisor 0.
        PLL_PLLDIV0_PLLDIV0_OF(0) // Define o divisor como 0. Isso significa que a saida do PLL nao sera dividida para o primeiro dominio de clock.
    ),
    /* Configuracoes dos Divisores 1, 2 e 3
        Iguais aos do DIV0
    */
    PLL_PLLDIV1_RMK(
        PLL_PLLDIV1_D1EN_ENABLED,
        PLL_PLLDIV1_PLLDIV1_OF(0)
    ),
    PLL_PLLDIV2_RMK(
        PLL_PLLDIV2_D2EN_ENABLED,
        PLL_PLLDIV2_PLLDIV2_OF(0)
    ),
    PLL_PLLDIV3_RMK(
        PLL_PLLDIV3_D3EN_ENABLED,
        PLL_PLLDIV3_PLLDIV3_OF(0)
    ),
    /* Divisor de Saida do Oscilador */
    PLL_OSCDIV1_RMK(
        PLL_OSCDIV1_OD1EN_DISABLED, // Desativa o divisor de saida do oscilador.
        PLL_OSCDIV1_OSCDIV1_OF(0) // Define o divisor como 0, o que significa que o oscilador tambem nao tera sua frequencia dividida.
    ),
    /* Configuracao de Wake Enable
        Este campo ativa as entradas de Wakeup Enable (WKEN) para permitir que diferentes fontes de wake-up ativem o PLL.
        Ativam todas as cinco fontes de wake-up para garantir que o PLL possa ser acordado por eventos externos.
    */
    PLL_WKEN_RMK(
        PLL_WKEN_WKEN4_ENABLED,
        PLL_WKEN_WKEN3_ENABLED,
        PLL_WKEN_WKEN2_ENABLED,
        PLL_WKEN_WKEN1_ENABLED,
        PLL_WKEN_WKEN0_ENABLED
    ),
    /* Modo de Clock
        Seleciona o Oscillator Output (OSCOUT) como o modo de saida do clock, provavelmente direcionando o clock de saida a partir do oscilador
     */
    PLL_CLKMD_RMK(
        PLL_CLKMD_CLKMD0_OSCOUT
    ),
    /* Configuracao da Saida de Clock */
    PLL_CLKOUTSR_RMK(
        PLL_CLKOUTSR_CLKOSEL_SYSCLK1, // Seleciona o SYSCLK1 como a fonte de clock para a saida do clock.
        PLL_CLKOUTSR_CLKOUTDIS_ENABLED // Habilita a saida de clock, ou seja, o sinal de clock sera emitido externamente.
    )
};

void initPLL(void){

    PLL_config(&myConfig);

    /*Parametros para PLL_setFreq sao:
        enable/mode, multiply, divider0, divider1, divider2, divider3, osc divider

        mode  = 1 significa PLL habilitado (non-bypass mode)
        mul   = 15 significa uma multiplicacao para 15
        div0  = 0 significa dividir por 1 usando PLLDIV0
        div1  = 3 significa dividir por 4 usando PLLDIV1
        div2  = 3 significa dividir por 4 usando PLLDIV2
        div3  = 3 significa dividir por 4 usando PLLDIV3
        oscdiv= 1 significa dividir por 2 usando OSCDIV1
    */

    /* Configura a frequencia necessaria para a CPU, perifericos Rapidos e Lentos e o EMIF */
    PLL_setFreq(1,15,0,3,3,3,1); // CPU @ 300 MHz, outros @ 75MHz
}
