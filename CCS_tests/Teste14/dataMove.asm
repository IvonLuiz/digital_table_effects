    .text                      	; Inicio da secao de codigo
    .global _dataMove          	; Tornar o rotulo _dataMove acessivel globalmente

XMIT_BUFF_SIZE .set 512        	; Definir constante para o tamanho do buffer

_dataMove:                     	; Rotulo para a funcao _dataMove
    add  #0, AR0              	; Inicializa o registrador AR0 para apontar para reception[N]
    add  #0, AR1              	; Inicializa o registrador AR1 para apontar para transmition[N]

    rpt  #XMIT_BUFF_SIZE-1    	; Configurar repeticao para XMIT_BUFF_SIZE-1 iteracoes
    mov  *AR0+, *AR1+         	; Mover dados de *AR0 (reception[i]) para *AR1 (transmition[i])
                              	; O sufixo + indica que os ponteiros AR0 e AR1 sao incrementados automaticamente apos a operacao.

    ret                       	; Retornar da funcao

    .end                      	; Fim do codigo
