#include <stdio.h>
#include "ezdsp5502.h"
#include "ezdsp5502_mcbsp.h"
#include "csl_dma.h"
#include "csl_mcbsp.h"

#include "dma.h"

#define N 256


Uint16 xmtEventId, rcvEventId;                  // Identificadores dos eventos de interrupcao gerados pelo DMA:
                                                // xmtEventId: Identificador para o canal de transmissao.
                                                // rcvEventId: Identificador para o canal de recepcao.

Int16 counter = 0;                              //
Int16 isActive = 0;

extern void VECSTART(void);                     // Declara uma referencia para a fun��o VECSTART, que aponta para o in�icio da tabela de vetores de interrupcao.
                                                // Usado para configurar as interrupcoes no codigo principal.

Uint16 old_intm;                                // Variavel para armazenar o estado anterior das interrupcoes globais.

interrupt void dmaXmtIsr(void);                 // Funcao chamada quando o DMA de transmissao conclui sua operacao.
interrupt void dmaRcvIsr(void);                 // Funcao chamada quando o DMA de recepcao conclui sua operacao.

// Configuracao do DMA para Transmissao (Tx)
DMA_Config dmaTx1Config = {
    DMA_DMACSDP_RMK(
        DMA_DMACSDP_DSTBEN_NOBURST,             // Sem burst de destino
        DMA_DMACSDP_DSTPACK_OFF,                // Sem empacotamento de destino
        DMA_DMACSDP_DST_PERIPH,                 // Destino: registrador de transmiss�o (DXR do McBSP)
        DMA_DMACSDP_SRCBEN_NOBURST,             // Sem burst de origem
        DMA_DMACSDP_SRCPACK_OFF,                // Sem empacotamento de origem
        DMA_DMACSDP_SRC_DARAMPORT0,             // Origem: mem�ria de dados (DARAM porta 0)
        DMA_DMACSDP_DATATYPE_16BIT              // Dados de 16 bits
    ),
    DMA_DMACCR_RMK(
        DMA_DMACCR_DSTAMODE_CONST,              // Endere�o de destino constante
        DMA_DMACCR_SRCAMODE_POSTINC,            // Endere�o de origem incrementado ap�s cada transfer�ncia
        DMA_DMACCR_ENDPROG_OFF,                 // Sem indica��o de fim de programa��o
        DMA_DMACCR_WP_DEFAULT,                  // Sem prote��o de escrita
        DMA_DMACCR_REPEAT_ALWAYS,               // Sem repeti��o autom�tica
        DMA_DMACCR_AUTOINIT_ON,                 // Auto inicializa��o ativada (essencial para ping-pong)
        DMA_DMACCR_EN_STOP,                     // Inicialmente desabilitado
        DMA_DMACCR_PRIO_HI,                     // Prioridade alta para este canal
        DMA_DMACCR_FS_DISABLE,                  // Sincroniza��o por elemento desabilitada
        DMA_DMACCR_SYNC_XEVT1                   // Sincroniza��o com evento de transmiss�o (XEVT1)
    ),
    DMA_DMACICR_RMK(
        DMA_DMACICR_AERRIE_OFF,                 // Sem interrup��o por erro de acesso
        DMA_DMACICR_BLOCKIE_OFF,                // Sem interrup��o de bloco completo
        DMA_DMACICR_LASTIE_OFF,                 // Sem interrup��o de �ltimo quadro
        DMA_DMACICR_FRAMEIE_ON,                 // Interrup��o habilitada ao final de cada quadro
        DMA_DMACICR_FIRSTHALFIE_OFF,            // Sem interrup��o de primeira metade
        DMA_DMACICR_DROPIE_OFF,                 // Sem interrup��o por evento descartado
        DMA_DMACICR_TIMEOUTIE_OFF               // Sem interrup��o por tempo esgotado
    ),
    (DMA_AdrPtr)&XmitBuffer1,                    // Endere�o de origem: Primeiro buffer de transmiss�o
    0,                                          // Endere�o superior da origem
    (DMA_AdrPtr)(MCBSP_ADDR(DXR11)),            // Endere�o de destino: Registrador DXR11 do McBSP
    0,                                          // Endere�o superior do destino
    N,                                // N�mero de elementos por quadro
    1,                                          // N�mero de quadros
    0,                                          // �ndice de quadro da origem
    0,                                          // �ndice de elemento da origem
    0,                                          // �ndice de quadro do destino
    0                                           // �ndice de elemento do destino
};

DMA_Config dmaTx2Config = {
    DMA_DMACSDP_RMK(
        DMA_DMACSDP_DSTBEN_NOBURST,             // Sem burst de destino
        DMA_DMACSDP_DSTPACK_OFF,                // Sem empacotamento de destino
        DMA_DMACSDP_DST_PERIPH,                 // Destino: registrador de transmiss�o (DXR do McBSP)
        DMA_DMACSDP_SRCBEN_NOBURST,             // Sem burst de origem
        DMA_DMACSDP_SRCPACK_OFF,                // Sem empacotamento de origem
        DMA_DMACSDP_SRC_DARAMPORT0,             // Origem: mem�ria de dados (DARAM porta 0)
        DMA_DMACSDP_DATATYPE_16BIT              // Dados de 16 bits
    ),
    DMA_DMACCR_RMK(
        DMA_DMACCR_DSTAMODE_CONST,              // Endere�o de destino constante
        DMA_DMACCR_SRCAMODE_POSTINC,            // Endere�o de origem incrementado ap�s cada transfer�ncia
        DMA_DMACCR_ENDPROG_OFF,                 // Sem indica��o de fim de programa��o
        DMA_DMACCR_WP_DEFAULT,                  // Sem prote��o de escrita
        DMA_DMACCR_REPEAT_ALWAYS,               // Sem repeti��o autom�tica
        DMA_DMACCR_AUTOINIT_ON,                 // Auto inicializa��o ativada (essencial para ping-pong)
        DMA_DMACCR_EN_STOP,                     // Inicialmente desabilitado
        DMA_DMACCR_PRIO_HI,                     // Prioridade alta para este canal
        DMA_DMACCR_FS_DISABLE,                  // Sincroniza��o por elemento desabilitada
        DMA_DMACCR_SYNC_XEVT1                   // Sincroniza��o com evento de transmiss�o (XEVT1)
    ),
    DMA_DMACICR_RMK(
        DMA_DMACICR_AERRIE_OFF,                 // Sem interrup��o por erro de acesso
        DMA_DMACICR_BLOCKIE_OFF,                // Sem interrup��o de bloco completo
        DMA_DMACICR_LASTIE_OFF,                 // Sem interrup��o de �ltimo quadro
        DMA_DMACICR_FRAMEIE_ON,                 // Interrup��o habilitada ao final de cada quadro
        DMA_DMACICR_FIRSTHALFIE_OFF,            // Sem interrup��o de primeira metade
        DMA_DMACICR_DROPIE_OFF,                 // Sem interrup��o por evento descartado
        DMA_DMACICR_TIMEOUTIE_OFF               // Sem interrup��o por tempo esgotado
    ),
    (DMA_AdrPtr)&XmitBuffer2,                    // Endere�o de origem: Segundo buffer de transmiss�o
    0,                                          // Endere�o superior da origem
    (DMA_AdrPtr)(MCBSP_ADDR(DXR11)),            // Endere�o de destino: Registrador DXR11 do McBSP
    0,                                          // Endere�o superior do destino
    N,                                // N�mero de elementos por quadro
    1,                                          // N�mero de quadros
    0,                                          // �ndice de quadro da origem
    0,                                          // �ndice de elemento da origem
    0,                                          // �ndice de quadro do destino
    0                                           // �ndice de elemento do destino
};

// Configuracao do DMA para Recepcao (Rx)
DMA_Config dmaRx1Config = {
    DMA_DMACSDP_RMK(
        DMA_DMACSDP_DSTBEN_NOBURST,             // Sem burst de destino, os dados s�o transferidos um por um.
        DMA_DMACSDP_DSTPACK_OFF,                // Sem empacotamento de destino
        DMA_DMACSDP_DST_DARAMPORT0,             // O destino da transfer�ncia � a mem�ria de dados DARAM (porta 0).
        DMA_DMACSDP_SRCBEN_NOBURST,             // Sem burst de origem
        DMA_DMACSDP_SRCPACK_OFF,                // Sem empacotamento de origem
        DMA_DMACSDP_SRC_PERIPH,                 // A origem da transfer�ncia � um perif�rico (DRR do McBSP).
        DMA_DMACSDP_DATATYPE_16BIT              // Dados transferidos com tamanho de 16 bits.
    ),
    DMA_DMACCR_RMK(
        DMA_DMACCR_DSTAMODE_POSTINC,            // Incrementa o endere�o de destino ap�s cada transfer�ncia.
        DMA_DMACCR_SRCAMODE_CONST,              // Endere�o de origem constante
        DMA_DMACCR_ENDPROG_OFF,                 // Bit de fim de programa��o
        DMA_DMACCR_WP_DEFAULT,                  // Sem prote��o de escrita
        DMA_DMACCR_REPEAT_ALWAYS,               // Sem repeti��o autom�tica
        DMA_DMACCR_AUTOINIT_ON,                 // Auto inicializa��o ligada (essencial para ping-pong)
        DMA_DMACCR_EN_STOP,                     // Desabilitado inicialmente
        DMA_DMACCR_PRIO_HI,                     // Alta prioridade
        DMA_DMACCR_FS_DISABLE,                  // Sincroniza��o por elemento
        DMA_DMACCR_SYNC_REVT1                   // Sincronizar com o evento de recep��o (REVT)
    ),
    DMA_DMACICR_RMK(
        DMA_DMACICR_AERRIE_OFF,                 // Sem interrup��o por erro de acesso
        DMA_DMACICR_BLOCKIE_OFF,                // Sem interrup��o de bloco completo
        DMA_DMACICR_LASTIE_OFF,                 // Sem interrup��o de �ltimo quadro
        DMA_DMACICR_FRAMEIE_ON,                 // Habilita interrup��o ao final de cada quadro recebido
        DMA_DMACICR_FIRSTHALFIE_OFF,            // Sem interrup��o de primeira metade
        DMA_DMACICR_DROPIE_OFF,                 // Sem interrup��o por evento descartado
        DMA_DMACICR_TIMEOUTIE_OFF               // Sem interrup��o por tempo esgotado
    ),
    (DMA_AdrPtr)(MCBSP_ADDR(DRR11)),            // Endere�o de origem (registrador DRR do McBSP).
    0,                                          // Endere�o superior da origem.
    (DMA_AdrPtr)&RcvBuffer1,                     // Endere�o inicial do primeiro buffer de recep��o.
    0,                                          // Endere�o superior do destino.
    N,                                // N�mero de elementos por quadro.
    1,                                          // N�mero de quadros.
    0,                                          // �ndice de quadro da origem.
    0,                                          // �ndice de elemento da origem.
    0,                                          // �ndice de quadro do destino.
    0                                           // �ndice de elemento do destino.
};

DMA_Config dmaRx2Config = {
    DMA_DMACSDP_RMK(
        DMA_DMACSDP_DSTBEN_NOBURST,             // Sem burst de destino, os dados s�o transferidos um por um.
        DMA_DMACSDP_DSTPACK_OFF,                // Sem empacotamento de destino
        DMA_DMACSDP_DST_DARAMPORT0,             // O destino da transfer�ncia � a mem�ria de dados DARAM (porta 0).
        DMA_DMACSDP_SRCBEN_NOBURST,             // Sem burst de origem
        DMA_DMACSDP_SRCPACK_OFF,                // Sem empacotamento de origem
        DMA_DMACSDP_SRC_PERIPH,                 // A origem da transfer�ncia � um perif�rico (DRR do McBSP).
        DMA_DMACSDP_DATATYPE_16BIT              // Dados transferidos com tamanho de 16 bits.
    ),
    DMA_DMACCR_RMK(
        DMA_DMACCR_DSTAMODE_POSTINC,            // Incrementa o endere�o de destino ap�s cada transfer�ncia.
        DMA_DMACCR_SRCAMODE_CONST,              // Endere�o de origem constante
        DMA_DMACCR_ENDPROG_OFF,                 // Bit de fim de programa��o
        DMA_DMACCR_WP_DEFAULT,                  // Sem prote��o de escrita
        DMA_DMACCR_REPEAT_ALWAYS,               // Sem repeti��o autom�tica
        DMA_DMACCR_AUTOINIT_ON,                 // Auto inicializa��o ligada (essencial para ping-pong)
        DMA_DMACCR_EN_STOP,                     // Desabilitado inicialmente
        DMA_DMACCR_PRIO_HI,                     // Alta prioridade
        DMA_DMACCR_FS_DISABLE,                  // Sincroniza��o por elemento
        DMA_DMACCR_SYNC_REVT1                   // Sincronizar com o evento de recep��o (REVT)
    ),
    DMA_DMACICR_RMK(
        DMA_DMACICR_AERRIE_OFF,                 // Sem interrup��o por erro de acesso
        DMA_DMACICR_BLOCKIE_OFF,                // Sem interrup��o de bloco completo
        DMA_DMACICR_LASTIE_OFF,                 // Sem interrup��o de �ltimo quadro
        DMA_DMACICR_FRAMEIE_ON,                 // Habilita interrup��o ao final de cada quadro recebido
        DMA_DMACICR_FIRSTHALFIE_OFF,            // Sem interrup��o de primeira metade
        DMA_DMACICR_DROPIE_OFF,                 // Sem interrup��o por evento descartado
        DMA_DMACICR_TIMEOUTIE_OFF               // Sem interrup��o por tempo esgotado
    ),
    (DMA_AdrPtr)(MCBSP_ADDR(DRR11)),            // Endere�o de origem (registrador DRR do McBSP).
    0,                                          // Endere�o superior da origem.
    (DMA_AdrPtr)&RcvBuffer2,                    // Endere�o inicial do primeiro buffer de recep��o.
    0,                                          // Endere�o superior do destino.
    N,                                          // N�mero de elementos por quadro.
    1,                                          // N�mero de quadros.
    0,                                          // �ndice de quadro da origem.
    0,                                          // �ndice de elemento da origem.
    0,                                          // �ndice de quadro do destino.
    0                                           // �ndice de elemento do destino.
};

DMA_Handle dmaReception, dmaTransmition;        // Declara dois handles de DMA:
                                                // dmaReception: Sera usado para gerenciar o canal de recepcao de dados (Rx).
                                                // dmaTransmition: Sera usado para gerenciar o canal de transmissao de dados (Tx).

volatile Uint16 transferComplete = FALSE;       // volatile: Garante que o compilador nao otimize acessos a esta variavel, pois seu valor pode ser alterado por interrupcoes.
                                                // transferComplete: Um indicador de status:
                                                // FALSE: Transferencia ainda nao concluida.
                                                // TRUE: Transferencia foi concluida.
                                                // Essa variavel pode ser usada para sincronizar o processamento entre o nucleo principal e os canais de DMA.


//volatile Int16 currentRxBuffer = 1; // Indica qual buffer de recep��o est� ativo
//volatile Int16 currentTxBuffer = 1; // Indica qual buffer de transmiss�o est� ativo
Int16 currentRxBuffer = 1; // Indica qual buffer de recep��o est� ativo
Int16 currentTxBuffer = 1; // Indica qual buffer de transmiss�o est� ativo

interrupt void dmaXmtIsr(void) {
    IRQ_disable(xmtEventId); // Desabilita temporariamente interrup��es

    if (currentTxBuffer == 1) {
        // Atualiza a configura��o para o pr�ximo buffer
        DMA_config(dmaTransmition, &dmaTx2Config);
        currentTxBuffer = 2;
    } else {
        DMA_config(dmaTransmition, &dmaTx1Config);
        currentTxBuffer = 1;
    }

    IRQ_clear(xmtEventId);
    IRQ_enable(xmtEventId); // Reabilita interrup��es
}


interrupt void dmaRcvIsr(void) {
    IRQ_disable(rcvEventId); // Desabilita temporariamente interrup��es

    if (currentRxBuffer == 1) {
        // Configura DMA para utilizar o pr�ximo buffer
        DMA_config(dmaReception, &dmaRx2Config);
        currentRxBuffer = 2; // Alterna para o pr�ximo buffer
    } else {
        DMA_config(dmaReception, &dmaRx1Config);
        currentRxBuffer = 1; // Alterna para o primeiro buffer
    }

    IRQ_clear(rcvEventId);
    IRQ_enable(rcvEventId); // Reabilita interrup��es
}


void configAudioDma (void)
{
    CSL_init();                                 // Inicializa a biblioteca CSL (Chip Support Library)

    IRQ_setVecs((Uint32)(&VECSTART));           // Configura a tabela de vetores de interrupcao apontando para o inicio, definido por VECSTART.

    // Configura��o do DMA para Transmiss�o
    dmaTx1Config.dmacssal = (DMA_AdrPtr)(((Uint32)&XmitBuffer1) << 1); // Primeiro buffer de transmiss�o
    dmaTx1Config.dmacdsal = (DMA_AdrPtr)(((Uint32)MCBSP_ADDR(DXR11)) << 1); // Registrador DXR11 do McBSP

    dmaTx2Config.dmacssal = (DMA_AdrPtr)(((Uint32)&XmitBuffer2) << 1); // Primeiro buffer de transmiss�o
    dmaTx2Config.dmacdsal = (DMA_AdrPtr)(((Uint32)MCBSP_ADDR(DXR11)) << 1); // Registrador DXR11 do McBSP

    // Configura��o do DMA para Recep��o
    dmaRx1Config.dmacssal = (DMA_AdrPtr)(((Uint32)MCBSP_ADDR(DRR11)) << 1); // Registrador DRR11 do McBSP
    dmaRx1Config.dmacdsal = (DMA_AdrPtr)(((Uint32)&RcvBuffer1) << 1); // Primeiro buffer de recep��o

    dmaRx2Config.dmacssal = (DMA_AdrPtr)(((Uint32)MCBSP_ADDR(DRR11)) << 1); // Registrador DRR11 do McBSP
    dmaRx2Config.dmacdsal = (DMA_AdrPtr)(((Uint32)&RcvBuffer2) << 1); // Segundo buffer de recep��o

    dmaTransmition = DMA_open(DMA_CHA2, DMA_OPEN_RESET);                        // DMA_CHA2 eh o canal para transmissao.
    DMA_config(dmaTransmition, &dmaTx1Config);                                  // Configura o canal

    dmaReception = DMA_open(DMA_CHA1, DMA_OPEN_RESET);                          // DMA_CHA1 eh o canal para recepcao.
    DMA_config(dmaReception, &dmaRx1Config);                                    // Configura o canal

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

    // Associa os IDs de eventos as rotinas de servi�o de interrupcao
    IRQ_plug(xmtEventId,&dmaXmtIsr);
    IRQ_plug(rcvEventId,&dmaRcvIsr);


    IRQ_globalEnable();                         // Reabilita as interrupcoes globais apos configurar os canais de DMA e associar as ISRs.

}

void startAudioDma (void)
{
    if(!isActive){
        DMA_start(dmaReception);
        DMA_start(dmaTransmition);
        EZDSP5502_MCBSP_init();
        isActive = 1;
    }
}

void stopAudioDma (void)
{
    DMA_stop(dmaTransmition);
    DMA_stop(dmaReception);
    isActive = 0;
}
