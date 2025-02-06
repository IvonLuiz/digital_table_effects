#include <stdio.h>
#include "ezdsp5502.h"
#include "ezdsp5502_mcbsp.h"
#include "csl_dma.h"
#include "csl_mcbsp.h"

#include "dma.h"

#define N 512                              // valor usado como o tamanho dos buffers de transmissao (transmition) e recepcao (reception).

Int16 counter = 0;                              //
Int16 isActive = 0;                             //

Uint16 xmtEventId, rcvEventId;                  // Identificadores dos eventos de interrupcao gerados pelo DMA:
                                                // xmtEventId: Identificador para o canal de transmissao.
                                                // rcvEventId: Identificador para o canal de recepcao.

extern void VECSTART(void);                     // Declara uma referencia para a função VECSTART, que aponta para o iníicio da tabela de vetores de interrupcao.
                                                // Usado para configurar as interrupcoes no codigo principal.

extern void dataMove(Int16 *, Int16 *);         // Declara uma funcao externa dataMove, que realiza a movimentacao de dados entre dois ponteiros (source para destination).
                                                // Usada para copiar ou processar dados dos buffers transmition e reception.

Uint16 old_intm;                                // Variavel para armazenar o estado anterior das interrupcoes globais.

interrupt void dmaXmtIsr(void);                 // Funcao chamada quando o DMA de transmissao conclui sua operacao.
interrupt void dmaRcvIsr(void);                 // Funcao chamada quando o DMA de recepcao conclui sua operacao.

void startAudioDma(void);
void stopAudioDma(void);

// Configuracao do DMA para Transmissao (Tx)
DMA_Config dmaTx1Config = {
    DMA_DMACSDP_RMK(
        DMA_DMACSDP_DSTBEN_NOBURST,             // Sem burst de destino, os dados sao transferidos um por um.
        DMA_DMACSDP_DSTPACK_OFF,                // Sem empacotamento de destino
        DMA_DMACSDP_DST_PERIPH,                 // Destino eh um periférico, no caso, o registrador de transmissao (DXR) do McBSP.
        DMA_DMACSDP_SRCBEN_NOBURST,             // Sem burst de origem
        DMA_DMACSDP_SRCPACK_OFF,                // Sem empacotamento de origem
        DMA_DMACSDP_SRC_DARAMPORT0,             // Define a origem como memoria de dados no barramento DARAM (porta 0).
        DMA_DMACSDP_DATATYPE_16BIT              // Especifica que os dados transferidos tem tamanho de 16 bits.
    ),
    DMA_DMACCR_RMK(
        DMA_DMACCR_DSTAMODE_CONST,              // Endereco de destino constante
        DMA_DMACCR_SRCAMODE_POSTINC,            // Endereco de origem com incremento
        DMA_DMACCR_ENDPROG_OFF,                 // Sem indicacao de fim de programacao
        DMA_DMACCR_WP_DEFAULT,                  // Sem protecao de escrita
        DMA_DMACCR_REPEAT_ALWAYS,               // Sem repeticao automatica
        DMA_DMACCR_AUTOINIT_ON,                 // Auto inicializacao ativada
        DMA_DMACCR_EN_STOP,                     // Transferencia inicialmente desabilitada
        DMA_DMACCR_PRIO_HI,                     // Prioridade alta para este canal
        DMA_DMACCR_FS_DISABLE,                  // Sincronizacao por elemento desabilitada
        DMA_DMACCR_SYNC_XEVT1                   // Sincronizacao com evento de transmissao (XEVT1)
    ),
    DMA_DMACICR_RMK(
        DMA_DMACICR_AERRIE_OFF,                 // Sem interrupcao por erro de acesso
        DMA_DMACICR_BLOCKIE_OFF,                // Sem interrupcao de bloco completo
        DMA_DMACICR_LASTIE_OFF,                 // Sem interrupcao de último quadro
        DMA_DMACICR_FRAMEIE_OFF,                // Sem interrupcao de quadro completo
        DMA_DMACICR_FIRSTHALFIE_OFF,            // Sem interrupcao de primeira metade
        DMA_DMACICR_DROPIE_OFF,                 // Sem interrupcao por evento descartado
        DMA_DMACICR_TIMEOUTIE_OFF               // Sem interrupcao por tempo esgotado
    ),
    (DMA_AdrPtr)&XmitBuffer1,                   // Endereco de origem (memoria de audio)
    0,                                          // Endereco superior da origem
    (DMA_AdrPtr)(MCBSP_ADDR(DXR11)),            // Endereco de destino (registrador DXR)
    0,                                          // Endereco superior do destino
    N,                                          // Numero de elementos por quadro
    1,                                          // Numero de quadros
    0,                                          // Indice de quadro da origem
    0,                                          // Indice de elemento da origem
    0,                                          // Indice de quadro do destino
    0                                           // Indice de elemento do destino
};

// Configuracao do DMA para Recepcao (Rx)
DMA_Config dmaRx1Config = {
    DMA_DMACSDP_RMK(
        DMA_DMACSDP_DSTBEN_NOBURST,             // Sem burst de destino, os dados sao transferidos um por um.
        DMA_DMACSDP_DSTPACK_OFF,                // Sem empacotamento de destino
        DMA_DMACSDP_DST_DARAMPORT0,             // Especifica que o destino da transferencia eh a memoria de dados DARAM (porta 0).
        DMA_DMACSDP_SRCBEN_NOBURST,             // Sem burst de origem
        DMA_DMACSDP_SRCPACK_OFF,                // Sem empacotamento de origem
        DMA_DMACSDP_SRC_PERIPH,                 // Especifica que a origem da transferencia eh um periferico, no caso, o registrador de recepcao (DRR) do McBSP.
        DMA_DMACSDP_DATATYPE_16BIT              // Define o tamanho dos dados transferidos como 16 bits.
    ),
    DMA_DMACCR_RMK(
        DMA_DMACCR_DSTAMODE_POSTINC,            // Incrementa o endereco de destino apos cada transferencia.
        DMA_DMACCR_SRCAMODE_CONST,              // Endereço de origem constante
        DMA_DMACCR_ENDPROG_OFF,                 // Bit de fim de programacao
        DMA_DMACCR_WP_DEFAULT,                  // Sem protecao de escrita
        DMA_DMACCR_REPEAT_ALWAYS,               // Sem repeticao automatica
        DMA_DMACCR_AUTOINIT_ON,                 // Auto inicializacao ligada
        DMA_DMACCR_EN_STOP,                     // Desabilitado inicialmente
        DMA_DMACCR_PRIO_HI,                     // Alta prioridade
        DMA_DMACCR_FS_DISABLE,                  // Sincronizacao por elemento
        DMA_DMACCR_SYNC_REVT1                   // Sincronizar com o evento de recepção (REVT)
    ),
    DMA_DMACICR_RMK(
        DMA_DMACICR_AERRIE_OFF,                 // Sem interrupcao por erro de acesso
        DMA_DMACICR_BLOCKIE_OFF,                // Sem interrupcao de bloco completo
        DMA_DMACICR_LASTIE_OFF,                 // Sem interrupcao de ultimo quadro
        DMA_DMACICR_FRAMEIE_ON,                 // Habilita interrupcao ao final de cada quadro recebido, util para processar os dados capturados em tempo real
        DMA_DMACICR_FIRSTHALFIE_OFF,            // Sem interrupcao de primeira metade
        DMA_DMACICR_DROPIE_OFF,                 // Sem interrupcao por evento descartado
        DMA_DMACICR_TIMEOUTIE_OFF               // Sem interrupcao por tempo esgotado
    ),
    (DMA_AdrPtr)(MCBSP_ADDR(DRR11)),            // Endereco de origem (registrador DRR)
    0,                                          // Endereco superior da origem
    (DMA_AdrPtr)&RcvBuffer1,                     // Endereco de destino (memória de áudio)
    0,                                          // Endereco superior do destino
    N,                                          // Numero de elementos por quadro
    1,                                          // Numero de quadros
    0,                                          // Indice de quadro da origem
    0,                                          // Indice de elemento da origem
    0,                                          // Indice de quadro do destino
    0                                           // Indice de elemento do destino
};

DMA_Handle dmaReception, dmaTransmition;        // Declara dois handles de DMA:
                                                // dmaReception: Sera usado para gerenciar o canal de recepcao de dados (Rx).
                                                // dmaTransmition: Sera usado para gerenciar o canal de transmissao de dados (Tx).

volatile Uint16 transferComplete = FALSE;       // volatile: Garante que o compilador nao otimize acessos a esta variavel, pois seu valor pode ser alterado por interrupcoes.
                                                // transferComplete: Um indicador de status:
                                                // FALSE: Transferencia ainda nao concluida.
                                                // TRUE: Transferencia foi concluida.
                                                // Essa variavel pode ser usada para sincronizar o processamento entre o nucleo principal e os canais de DMA.

Int16 currentTxBuffer = 1;
Int16 currentRxBuffer = 1;

interrupt void dmaXmtIsr(void) {

}

Int16 readyBuffer = 0;  // Sinalizador para indicar qual buffer de recepção está pronto

interrupt void dmaRcvIsr(void) {

}


void configAudioDma (void)
{
    CSL_init();                                 // Inicializa a biblioteca CSL (Chip Support Library)

    IRQ_setVecs((Uint32)(&VECSTART));           // Configura a tabela de vetores de interrupcao apontando para o inicio, definido por VECSTART.


    dmaTx1Config.dmacssal = (DMA_AdrPtr)(((Uint32)&XmitBuffer1) << 1);           // O buffer transmition contem os dados que serao enviados.
    dmaTx1Config.dmacdsal = (DMA_AdrPtr)(((Uint32)MCBSP_ADDR(DXR11)) << 1);      // O registrador DXR11 do McBSP transmite os dados para o codec de audio.

    dmaRx1Config.dmacssal = (DMA_AdrPtr)(((Uint32)MCBSP_ADDR(DRR11)) << 1);      // O registrador DRR11 do McBSP, que recebe dados do codec de audio.
    dmaRx1Config.dmacdsal = (DMA_AdrPtr)(((Uint32)&RcvBuffer1) << 1);             // O buffer reception, onde os dados recebidos serao armazenados.

    dmaTransmition = DMA_open(DMA_CHA2, DMA_OPEN_RESET);                        // DMA_CHA2 eh o canal para transmissao.
    DMA_config(dmaTransmition, &dmaTx1Config);                                   // Configura o canal

    dmaReception = DMA_open(DMA_CHA1, DMA_OPEN_RESET);                          // DMA_CHA1 eh o canal para recepcao.
    DMA_config(dmaReception, &dmaRx1Config);                                     // Configura o canal

    // Obtem os IDs de eventos de interrupcao associados aos canais DMA
    xmtEventId = DMA_getEventId(dmaTransmition);
    rcvEventId = DMA_getEventId(dmaReception);

    old_intm = IRQ_globalDisable();             // Desabilita temporariamente todas as interrupcoes globais.

    // Limpa qualquer interrupcao pendente associada aos canais de transmissao e recepcao.
    IRQ_clear(xmtEventId);
    IRQ_clear(rcvEventId);

    // Habilita os eventos de interrupcao para os canais de transmissao e recepcao.
    IRQ_enable(xmtEventId);
    IRQ_enable(rcvEventId);

    // Associa os IDs de eventos as rotinas de serviço de interrupcao
    IRQ_plug(xmtEventId,&dmaXmtIsr);
    IRQ_plug(rcvEventId,&dmaRcvIsr);


    IRQ_globalEnable();                         // Reabilita as interrupcoes globais apos configurar os canais de DMA e associar as ISRs.

}

void startAudioDma(void)
{
    if(!isActive){
        DMA_start(dmaReception);
        DMA_start(dmaTransmition);
        EZDSP5502_MCBSP_init();
        isActive = 1;
    }
}

void stopAudioDma(void)
{
    DMA_stop(dmaTransmition);
    DMA_stop(dmaReception);
    EZDSP5502_MCBSP_close();
    isActive = 0;
}
