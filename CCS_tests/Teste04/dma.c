/*
 * dma.c
 *
 *  Created on: 13 de out de 2024
 *      Author: Heverton Reis
 */
#include "dma.h"
#include"ezdsp5502.h"
#include"ezdsp5502_mcbsp.h"
#include "csl_dma.h"

DMA_Config myconfig = {
    DMA_DMACSDP_RMK(
        DMA_DMACSDP_DSTBEN_NOBURST , // Destination burst
        DMA_DMACSDP_DSTPACK_OFF,     // Destination packing
        DMA_DMACSDP_DST_PERIPH ,     // Source selection
        DMA_DMACSDP_SRCBEN_NOBURST , // Source burst
        DMA_DMACSDP_SRCPACK_OFF,     // Source packing
        DMA_DMACSDP_SRC_DARAMPORT1 , // Source selection
        DMA_DMACSDP_DATATYPE_16BIT   // Data type
    ), /* DMACSDP */

    DMA_DMACCR_RMK(
        DMA_DMACCR_DSTAMODE_CONST,   // Destination address mode
        DMA_DMACCR_SRCAMODE_POSTINC, // Source address mode
        DMA_DMACCR_ENDPROG_OFF,      // End of programmation bit
        DMA_DMACCR_WP_DEFAULT,
        DMA_DMACCR_REPEAT_ALWAYS,    // Repeat condition
        DMA_DMACCR_AUTOINIT_ON,      // Auto initialization bit
        DMA_DMACCR_EN_STOP,          // Channel enable
        DMA_DMACCR_PRIO_HI,          // Channel priority
        DMA_DMACCR_FS_ELEMENT,       // Frame\Element Sync
        DMA_DMACCR_SYNC_XEVT1        // Synchronization control
    ), /* DMACCR */

    DMA_DMACICR_RMK(
        DMA_DMACICR_AERRIE_OFF,
        DMA_DMACICR_BLOCKIE_OFF ,    // Whole block interrupt enable
        DMA_DMACICR_LASTIE_OFF,      // Last frame Interrupt enable
        DMA_DMACICR_FRAMEIE_OFF,     // Whole frame interrupt enable
        DMA_DMACICR_FIRSTHALFIE_OFF, // HAlf frame interrupt enable
        DMA_DMACICR_DROPIE_OFF,      // Sync event drop interrupt enable
        DMA_DMACICR_TIMEOUTIE_OFF    // Time out inetrrupt enable
    ), /* DMACICR */

    (DMA_AdrPtr)0x5000, // DMACSSAL - DMA source set to random location (Set to sine array later)
    0,                  // DMACSSAU
    (DMA_AdrPtr)0x5804, // DMACDSAL - DMA destination is DXR1
    0,                  // DMACDSAU
    96,                 // DMACEN  - 96 elements (48 for each channel)
    1,                  // DMACFN  - Single frame
    0,                  // DMACSFI - Source frame index
    0,                  // DMACSEI - Source element index
    0,                  // DMACDFI - Destination frame index
    2                   // DMACDEI - Destination element index
};

/* Sinal de -3dB 1 KHz amostrado a 48 KHz para os canais E/D */
Uint16 Sinal_1K[96] = {
0x0000,0x0000,0x0B6C,0x0B6C,0x16A6,0x16A6,0x217C,0x217C,0x2BC0,0x2BC0,0x3544,0x3544,0x3DDF,0x3DDF,0x456B,0x456B,
0x4BC7,0x4BC7,0x50D7,0x50D7,0x5485,0x5485,0x56C0,0x56C0,0x5780,0x5780,0x56C0,0x56C0,0x5485,0x5485,0x50D7,0x50D7,
0x4BC7,0x4BC7,0x456B,0x456B,0x3DDF,0x3DDF,0x3544,0x3544,0x2BC0,0x2BC0,0x217C,0x217C,0x16A6,0x16A6,0x0B6C,0x0B6C,
0x0000,0x0000,0xF494,0xF494,0xE95A,0xE95A,0xDE84,0xDE84,0xD440,0xD440,0xCABC,0xCABC,0xC221,0xC221,0xBA95,0xBA95,
0xB439,0xB439,0xAF29,0xAF29,0xAB7B,0xAB7B,0xA940,0xA940,0xA880,0xA880,0xA940,0xA940,0xAB7B,0xAB7B,0xAF29,0xAF29,
0xB439,0xB439,0xBA95,0xBA95,0xC221,0xC221,0xCABC,0xCABC,0xD440,0xD440,0xDE84,0xDE84,0xE95A,0xE95A,0xF494,0xF494
};

/* Sinal de -3 dB 2 KHz amostrado a 48 KHz para os canais E/D */
Int16 Sinal_2K[96] = {
0x0000,0x0000,0x16A6,0x16A6,0x2BC0,0x2BC0,0x3DDF,0x3DDF,0x4BC7,0x4BC7,0x5485,0x5485,0x5780,0x5780,0x5485,0x5485,
0x4BC7,0x4BC7,0x3DDF,0x3DDF,0x2BC0,0x2BC0,0x16A6,0x16A6,0x0000,0x0000,0xE95A,0xE95A,0xD440,0xD440,0xC221,0xC221,
0xB439,0xB439,0xAB7B,0xAB7B,0xA880,0xA880,0xAB7B,0xAB7B,0xB439,0xB439,0xC221,0xC221,0xD440,0xD440,0xE95A,0xE95A,
0x0000,0x0000,0x16A6,0x16A6,0x2BC0,0x2BC0,0x3DDF,0x3DDF,0x4BC7,0x4BC7,0x5485,0x5485,0x5780,0x5780,0x5485,0x5485,
0x4BC7,0x4BC7,0x3DDF,0x3DDF,0x2BC0,0x2BC0,0x16A6,0x16A6,0x0000,0x0000,0xE95A,0xE95A,0xD440,0xD440,0xC221,0xC221,
0xB439,0xB439,0xAB7B,0xAB7B,0xA880,0xA880,0xAB7B,0xAB7B,0xB439,0xB439,0xC221,0xC221,0xD440,0xD440,0xE95A,0xE95A,
};

DMA_Handle myhDma;
Uint8 dmaState = 0; // Estado do DMA

void configAudioDma (void)
{
    /* Define o endereco de origem de onde os dados do Sinal_1K serao lidos */
    myconfig.dmacssal = (DMA_AdrPtr)(((Uint32)&Sinal_1K) << 1);

    myhDma = DMA_open(DMA_CHA0, 0);  // Abre o Canal 0 do DMA para transferencias de dados.
    DMA_config(myhDma, &myconfig);   // Passa a configuracao do canal DMA para o handle
}

void startAudioDma (void)
{
    DMA_start(myhDma); // Comeca a transferencia
}

void changeTone (void)
{
    if(dmaState == 0) // Significa que o tom atual eh de 1 kHz e sera mudado para 2 kHz.
    {
        DMA_close(myhDma);  // Fecha o DMA

        /* Configura o endereco para o Sinal_2K */
        myconfig.dmacssal = (DMA_AdrPtr)(((Uint32)&Sinal_2K) << 1);

        myhDma = DMA_open(DMA_CHA0, 0);  // Abre o Canal 0 do DMA
        DMA_config(myhDma, &myconfig);   // Configura o Canal
        DMA_start(myhDma);               // Inicia o DMA
        EZDSP5502_MCBSP_init( );         // Reinicializa o McBSP
        dmaState = 1;                    // Muda o estado
    }
    else // Significa que o tom atual eh de 2 kHz e sera mudado para 1 kHz.
    {
        DMA_close(myhDma);  // Fecha o DMA

        /* Configura o endereco para o Sinal_1K */
        myconfig.dmacssal = (DMA_AdrPtr)(((Uint32)&Sinal_1K) << 1);

        myhDma = DMA_open(DMA_CHA0, 0);  // Abre o Canal 0 do DMA
        DMA_config(myhDma, &myconfig);   // Configura o Canal
        DMA_start(myhDma);               // Inicia o DMA
        EZDSP5502_MCBSP_init( );         // Reinicializa o McBSP
        dmaState = 0;                    // Muda o estado
    }
}
