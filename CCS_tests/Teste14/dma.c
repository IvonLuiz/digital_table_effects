#include <stdio.h>
#include "ezdsp5502.h"
#include "ezdsp5502_mcbsp.h"
#include "csl_dma.h"
#include "csl_mcbsp.h"

#define N 512                                   // valor usado como o tamanho dos buffers de transmissao (transmition) e recepcao (reception).

#pragma DATA_SECTION(transmition,"dmaTest")     // Especifica que o buffer transmition sera armazenado na secao de memoria chamada "dmaTest".
#pragma DATA_ALIGN(transmition, 8)              // Garante que transmition estara alinhado em um limite de 8 bytes na memoria.
Int16 transmition[N];                           // buffer usado para armazenar os dados transmitidos pelo DMA.

#pragma DATA_SECTION(reception,"dmaTest")       // Especifica que o buffer reception sera armazenado na secao de memoria chamada "dmaTest".
#pragma DATA_ALIGN(reception, 8);               // Garante que reception estara alinhado em um limite de 8 bytes na memoria.
Int16 reception[N];                             // buffer usado para armazenar os dados recebidos pelo DMA.

Int16 counter = 0;                              //
Int16 isActive = 0;                             //

Uint16 xmtEventId, rcvEventId;                  // Identificadores dos eventos de interrupcao gerados pelo DMA:
                                                // xmtEventId: Identificador para o canal de transmissao.
                                                // rcvEventId: Identificador para o canal de recepcao.

extern void VECSTART(void);                     // Declara uma referencia para a fun��o VECSTART, que aponta para o in�icio da tabela de vetores de interrupcao.
                                                // Usado para configurar as interrupcoes no codigo principal.

extern void dataMove(Int16 *, Int16 *);         // Declara uma funcao externa dataMove, que realiza a movimentacao de dados entre dois ponteiros (source para destination).
                                                // Usada para copiar ou processar dados dos buffers transmition e reception.

Uint16 old_intm;                                // Variavel para armazenar o estado anterior das interrupcoes globais.

interrupt void dmaXmtIsr(void);                 // Funcao chamada quando o DMA de transmissao conclui sua operacao.
interrupt void dmaRcvIsr(void);                 // Funcao chamada quando o DMA de recepcao conclui sua operacao.


//DMA_Handle dmaHandleTx, dmaHandleRx;            // Declara dois handles para os canais DMA:
                                                // dmaHandleTx: Handle para o canal de transmissao.
                                                // dmaHandleRx: Handle para o canal de recepcao.
                                                // Estes handles s�o usados para configurar, iniciar e gerenciar os canais DMA.

// Configuracao do DMA para Transmissao (Tx)
DMA_Config dmaTxConfig = {
    DMA_DMACSDP_RMK(
        DMA_DMACSDP_DSTBEN_NOBURST,             // Sem burst de destino, os dados sao transferidos um por um.
        DMA_DMACSDP_DSTPACK_OFF,                // Sem empacotamento de destino
        DMA_DMACSDP_DST_PERIPH,                 // Destino eh um perif�rico, no caso, o registrador de transmissao (DXR) do McBSP.
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
        DMA_DMACICR_LASTIE_OFF,                 // Sem interrupcao de �ltimo quadro
        DMA_DMACICR_FRAMEIE_OFF,                // Sem interrupcao de quadro completo
        DMA_DMACICR_FIRSTHALFIE_OFF,            // Sem interrupcao de primeira metade
        DMA_DMACICR_DROPIE_OFF,                 // Sem interrupcao por evento descartado
        DMA_DMACICR_TIMEOUTIE_OFF               // Sem interrupcao por tempo esgotado
    ),
    (DMA_AdrPtr)&transmition,                   // Endereco de origem (memoria de audio)
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
DMA_Config dmaRxConfig = {
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
        DMA_DMACCR_SRCAMODE_CONST,              // Endere�o de origem constante
        DMA_DMACCR_ENDPROG_OFF,                 // Bit de fim de programacao
        DMA_DMACCR_WP_DEFAULT,                  // Sem protecao de escrita
        DMA_DMACCR_REPEAT_ALWAYS,               // Sem repeticao automatica
        DMA_DMACCR_AUTOINIT_ON,                 // Auto inicializacao ligada
        DMA_DMACCR_EN_STOP,                     // Desabilitado inicialmente
        DMA_DMACCR_PRIO_HI,                     // Alta prioridade
        DMA_DMACCR_FS_DISABLE,                  // Sincronizacao por elemento
        DMA_DMACCR_SYNC_REVT1                   // Sincronizar com o evento de recep��o (REVT)
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
    (DMA_AdrPtr)&reception,                     // Endereco de destino (mem�ria de �udio)
    0,                                          // Endereco superior do destino
    1,                                          // Numero de elementos por quadro
    N,                                          // Numero de quadros
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

interrupt void dmaXmtIsr(void) {

}

interrupt void dmaRcvIsr(void) {

    IRQ_disable(rcvEventId);                    // Desabilita temporariamente o evento de interrupcao associado ao canal de recepcao.
                                                // Evita interrupcoes aninhadas ou interferencias enquanto a rotina de servico de interrupcao esta em execucao.

    dataMove(reception,transmition);            // Chama uma funcao externa dataMove para mover os dados do buffer de recepcao (reception) para o buffer de transmissao (transmition).

    IRQ_enable(rcvEventId);                     // Reabilita o evento de interrupcao associado ao canal de recepcao apos a conclusao do processamento.
}

void configAudioDma (void)
{
    CSL_init();                                 // Inicializa a biblioteca CSL (Chip Support Library)

    int i = 0;
    for (i = 0; i <= N; i++) {                  // Garante que os buffers estejam limpos antes de qualquer operacao de transmissao ou recepcao.
           transmition[i] =  0;
           reception[i] = 0;
       }

    IRQ_setVecs((Uint32)(&VECSTART));           // Configura a tabela de vetores de interrupcao apontando para o inicio, definido por VECSTART.


    dmaTxConfig.dmacssal = (DMA_AdrPtr)(((Uint32)&transmition) << 1);           // O buffer transmition contem os dados que serao enviados.
    dmaTxConfig.dmacdsal = (DMA_AdrPtr)(((Uint32)MCBSP_ADDR(DXR11)) << 1);      // O registrador DXR11 do McBSP transmite os dados para o codec de audio.

    // << 1: Os endere�os do DMA sao em bytes, enquanto os enderecos do compilador estao em palavras.
    // Este deslocamento converte o endereco de palavra para endereco de byte.

    dmaRxConfig.dmacssal = (DMA_AdrPtr)(((Uint32)MCBSP_ADDR(DRR11)) << 1);      // O registrador DRR11 do McBSP, que recebe dados do codec de audio.
    dmaRxConfig.dmacdsal = (DMA_AdrPtr)(((Uint32)&reception) << 1);             // O buffer reception, onde os dados recebidos serao armazenados.

    dmaTransmition = DMA_open(DMA_CHA2, DMA_OPEN_RESET);                        // DMA_CHA2 eh o canal para transmissao.
    DMA_config(dmaTransmition, &dmaTxConfig);                                   // Configura o canal

    dmaReception = DMA_open(DMA_CHA1, DMA_OPEN_RESET);                          // DMA_CHA1 eh o canal para recepcao.
    DMA_config(dmaReception, &dmaRxConfig);                                     // Configura o canal

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
