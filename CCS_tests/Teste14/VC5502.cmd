-stack    0x2000      	// Define o tamanho da pilha principal como 0x2000 bytes (8 KB).
						// Utilizado para armazenar variaveis locais e enderecos de retorno.

-sysstack 0x1000      	// Define o tamanho da pilha secundaria como 0x1000 bytes (4 KB).
						// Usada em situacoes especificas, como contextos de interrupcao

-heap     0x2000      	// Define o tamanho da area de heap como 0x2000 bytes (8 KB).
						// Usada para alocacao dinamica de memoria

-c                    	// Especifica que variaveis inicializadas devem ser automaticamente
						// configuradas em tempo de execucao.

-u _Reset             	// Força a inclusao da rotina de tratamento de interrupcao de reset no binario final.

MEMORY
{
	PAGE 0:  			// Espaco unificado de programa e dados.

		MMR    (RWIX): origin = 0x000000, length = 0x0000C0  	// Area de registradores de memoria mapeada (Memory-Mapped Registers),
																// usada para controle de periféricos.
		BTRSVD (RWIX): origin = 0x0000C0, length = 0x000240  	// Reservado para o carregador de inicializacao.
		DARAM  (RWIX): origin = 0x000300, length = 0x00FB00  	// Memoria DARAM (Dual-Access RAM), usada para armazenar secoes criticas de codigo e dados.
	  	VECS   (RWIX): origin = 0x00FE00, length = 0x000200   	// Tabela de vetores de interrupcao (256 bytes).
	  	// Areas de memoria externa mapeadas em diferentes chip selects.
	  	CE0          : origin = 0x010000, length = 0x3f0000
	  	CE1          : origin = 0x400000, length = 0x400000
	  	CE2          : origin = 0x800000, length = 0x400000
	  	CE3          : origin = 0xC00000, length = 0x3F8000
	  	PDROM   (RIX): origin = 0xFF8000, length = 0x008000  	// ROM programavel para armazenamento de firmware.

 	PAGE 2:  			// Espaco de 64K palavras para dispositivos de E/S mapeados na memoria.

  		IOPORT (RWI) : origin = 0x000000, length = 0x020000
}

SECTIONS
{
   .text     >  DARAM align(32) fill = 20h { * (.text)  }

	// Pilhas primaria e secundaria, usadas para chamadas de funcao e interrupcoes.
   .stack    >  DARAM align(32) fill = 00h
   .sysstack >  DARAM align(32) fill = 00h

   // Dados do Chip Support Library (CSL), geralmente contendo configuracoes de perifericos.
   .csldata  >  DARAM align(64) fill = 00h

   .data     >  DARAM align(32) fill = 00h 						// Variaveis globais inicializadas.
   .bss      >  DARAM align(32) fill = 00h 						// Variaveis globais nao inicializadas.

   .const    >  DARAM align(32) fill = 00h 						// Dados constantes, como tabelas de lookup ou strings
   .sysmem   >  DARAM											// ÁArea para alocacao dinamica (ex.: malloc).

   // Dados auxiliares para inicializacoes automaticas e tabelas de instrucoes switch.
   .switch   >  DARAM
   .cinit    >  DARAM
   .pinit    >  DARAM align(32) fill = 00h

   // Buffers para E/S em C e argumentos para main()
   .cio      >  DARAM align(32) fill = 00h
   .args     >  DARAM align(32) fill = 00h

	.vectors  >  VECS 											// Tabela de vetores de interrupcao, posicionada em VECS

	dmaTest    >  DARAM align(32) fill = 00h 					// Alocado em DARAM, usado para buffers associados ao DMA

   .ioport   >  IOPORT PAGE 2 									// Variaveis de E/S mapeadas na memoria, colocadas na area IOPORT de PAGE 2
}
