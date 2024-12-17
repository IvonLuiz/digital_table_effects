/*
 * dma.h
 *
 *  Created on: 15 de dez de 2024
 *      Author: Heverton Reis
 */

#ifndef DMA_H_
#define DMA_H_

extern Int16 RcvBuffer1[];
extern Int16 RcvBuffer2[];
extern Int16 XmitBuffer1[];
extern Int16 XmitBuffer2[];

extern Int16 currentRxBuffer; // Indica qual buffer de recepção está ativo
extern Int16 currentTxBuffer; // Indica qual buffer de transmissão está ativo

#endif /* DMA_H_ */
