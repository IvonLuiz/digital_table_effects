#ifndef DMA_H_
#define DMA_H_

extern Int16 RcvBuffer1[];
extern Int16 RcvBuffer2[];
extern Int16 XmitBuffer1[];
extern Int16 XmitBuffer2[];

extern Int16 currentRxBuffer; // Indica qual buffer de recep��o est� ativo
extern Int16 currentTxBuffer; // Indica qual buffer de transmiss�o est� ativo

extern Int16 readyBuffer;

#endif /* DMA_H_ */
