/*
 * timer.c
 *
 *  Created on: 13 de out de 2024
 *      Author: Heverton Reis
 */
#include "timer.h"
#include "ezdsp5502.h"
#include "csl_gpt.h"
#include "csl_irq.h"

GPT_Handle myhGpt;
Uint16 gpt12Evt_Id;

Uint16 timerFlag = 0;  // Flag que indica interpucao de timer
Uint8 timerState = 0;  // Indica o Estado do Timer

GPT_Config myGptCfg = 
{
    0,
    GPT_GPTGPINT_RMK(
        GPT_GPTGPINT_TIN1INV_DEFAULT,
        GPT_GPTGPINT_TIN1INT_DEFAULT
    ),
    GPT_GPTGPEN_RMK(
        GPT_GPTGPEN_TOUT1EN_DEFAULT,
        GPT_GPTGPEN_TIN1EN_DEFAULT  
    ),
    GPT_GPTGPDIR_RMK(
        GPT_GPTGPDIR_TOUT1DIR_DEFAULT,
        GPT_GPTGPDIR_TIN1DIR_DEFAULT
    ),
    GPT_GPTGPDAT_RMK(
        GPT_GPTGPDAT_TOUT1DAT_DEFAULT,
        GPT_GPTGPDAT_TIN1DAT_DEFAULT
    ),
    0x68C0, //PRD1
    0x0478, //PRD2
    0xFFFF, //PRD3
    0xFFFF, //PRD4
    GPT_GPTCTL1_RMK(
        GPT_GPTCTL1_TIEN_DEFAULT,
        GPT_GPTCTL1_CLKSRC_DEFAULT,
        GPT_GPTCTL1_ENAMODE_CONTINUOUS,
        GPT_GPTCTL1_PWID_DEFAULT,
        GPT_GPTCTL1_CP_DEFAULT,
        GPT_GPTCTL1_INVIN_DEFAULT,
        GPT_GPTCTL1_INVOUT_DEFAULT          
    ),
    GPT_GPTCTL2_RMK(
        GPT_GPTCTL2_TIEN_DEFAULT,
        GPT_GPTCTL2_CLKSRC_DEFAULT,
        GPT_GPTCTL2_ENAMODE_ONCE,
        GPT_GPTCTL2_PWID_DEFAULT,
        GPT_GPTCTL2_CP_DEFAULT,
        GPT_GPTCTL2_INVIN_DEFAULT,
        GPT_GPTCTL2_INVOUT_DEFAULT
    ),
    GPT_GPTGCTL1_RMK(
        GPT_GPTGCTL1_TDDR34_DEFAULT,
        GPT_GPTGCTL1_PSC34_DEFAULT,
        GPT_GPTGCTL1_TIMMODE_32BIT_DUAL,
        GPT_GPTGCTL1_TIM34RS_IN_RESET,
        GPT_GPTGCTL1_TIM12RS_IN_RESET
    )       
};

interrupt void gpt0Isr(void)
{
    timerFlag=1;
}

void initTimer0(void){

    IRQ_setVecs(0x7F00);    // Aponta para a tabela de vetores de interrupcao.
    IRQ_globalDisable();    /* Desabilita interrupcoes globalmente. 
        Isso impede que qualquer interrupcao ocorra enquanto o temporizador esta sendo configurado
    */ 
    
    /* Abre e reseta o Timer 0 (GPT0). */
    myhGpt = GPT_open(GPT_DEV0, GPT_OPEN_RESET);
    /* Obtem o Event ID do temporizador GPT0. 
    *   O Event ID eh um identificador que sera usado para associar a interrupcao gerada
    *   pelo temporizador ao tratamento apropriado. 
    */
    gpt12Evt_Id = GPT_getEventId(myhGpt);  
    IRQ_clear(gpt12Evt_Id);                // Limpa a interrupcao pendente.
    /* A funcao IRQ_plug() liga o Event ID do temporizador a rotina de interrupcao 
    *   gpt0Isr (Interrupt Service Routine), ou seja, essa funcao sera chamada sempre que 
    *   o timer gerar uma interrupcao.
    */
    IRQ_plug(gpt12Evt_Id, &gpt0Isr);
    IRQ_enable(gpt12Evt_Id);               // Habilita a interrupcao do temporizador.
    IRQ_globalEnable();                    // Habilita as interrupcoes globalmente.
    GPT_config(myhGpt, &myGptCfg);         // Configura o temporizador com as opcoes definidas na estrutura myGptCfg.
}

void startTimer0(void)
{
    GPT_start12(myhGpt);
}

void changeTimer(void)
{
    if(timerState == 0)
    {
        GPT_stop12(myhGpt);                 // Para o Timer0
        GPT_RSETH(myhGpt,GPTPRD1, 0x1A30);  //
        GPT_RSETH(myhGpt,GPTPRD2, 0x011E);  // Reseta o periodo para
        GPT_RSETH(myhGpt,GPTCNT1, 0x0000);  //
        GPT_RSETH(myhGpt,GPTCNT2, 0x0000);  // Reseta a contagem para 0
        GPT_start12(myhGpt);                // Inicia o Timer
        timerState = 1;                     // Muda o estado
    }
    else
    {
        GPT_stop12(myhGpt);                 // Para o Timer0
        GPT_RSETH(myhGpt,GPTPRD1, 0x68C0);  //
        GPT_RSETH(myhGpt,GPTPRD2, 0x0478);  // Reseta o periodo para
        GPT_RSETH(myhGpt,GPTCNT1, 0x0000);  //
        GPT_RSETH(myhGpt,GPTCNT2, 0x0000);  // Reseta a contagem para 0
        GPT_start12(myhGpt);;               // Inicia o Timer
        timerState = 0;                     // Muda o estado
    }
}
