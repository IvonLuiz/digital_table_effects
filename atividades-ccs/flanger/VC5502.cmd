-stack 		0x2000 /* Tamanho da pilha primaria      */
-sysstack 	0x1000 /* Tamanho da pilha secundaria    */
-heap		0x2000 /* Tamanho da area de heap        */
-c				   /* Utiliza as convensoes de linking do C: inicializa
					* variaveis em tempo de execucao */
-u_Reset           /* Forca o carregamento do Gerenciador de Interrupcao
					* de reset                       */
MEMORY
{
 	MMR	  	(RW) : origin = 0000000h length = 0000c0h /* MMRs */
 	DARAM 	(RW) : origin = 00000c0h length = 00ff40h /*On-chipDARAM*/
 	SARAM 	(RW) : origin = 0030000h length = 01e000h /*On-chipSARAM*/
 	SAROM_0 (RX) : origin = 0fe0000h length = 008000h /*On-chipROM0*/
 	SAROM_1	(RX) : origin = 0fe8000h length = 008000h /*On-chipROM1*/
 	SAROM_2	(RX) : origin = 0ff0000h length = 008000h /*On-chipROM2*/
 	SAROM_3	(RX) : origin = 0ff8000h length = 008000h /*On-chipROM3*/
}

SECTIONS
{
	vectors (NOLOAD)
	.bss		   : > DARAM
 	vector		   : > DARAM	ALIGN = 256
 	.stack         : > DARAM
 	.sysstack      : > DARAM
 	.sysmem        : > DARAM
 	.text          : > DARAM
 	.data          : > DARAM
 	.cinit         : > DARAM
 	.const         : > DARAM
 	.cio           : > DARAM
 	.usect         : > DARAM
 	.switch        : > DARAM
}
