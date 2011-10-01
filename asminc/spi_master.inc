;*****************************************************************
;*	File: spi_master.inc
;*	
;*	Rutinas de control de SPI en modo MASTER
;*	
;*	Autores:
;*			- Rodriguez Cañete, Macarena
;*			- Pepe, Ezequiel Ignacio
;*			- Douce Suárez, Cristian Gabriel
;*	
;*	
;*	
;*	
;*	
;*	
;*	
;*	
;*****************************************************************

;*****************************************************************
;*	CONSTANTES
;*****************************************************************
	.equ	pCSS	=	1
	.equ	pSS		=	2
	.equ	pMOSI	=	3
	.equ	pMISO	=	4
	.equ	pSCK	=	5
	.equ	cnf_mst =	0b01110001



;*****************************************************************
;*	Configuración de la comunicación SPI en MASTER
;*****************************************************************
SPI_Minit:
		;*	Set de SCK, MOSI y ~SS como salidas y MISO como
		;*	entrada
		sbi		DDRB,pMOSI
		sbi		DDRB,pSCK
		sbi		DDRB,pSS
		;cbi	DDRB,pCSS
		;*	Habilita comunicación SPI como MASTER a frecuencia
		;*	de clock de f/16
		ldi 	tmp, cnf_mst
		out 	SPCR, tmp
		in 		tmp, SPSR
		in 		tmp, SPDR
		ret


;*****************************************************************
;*	Transmisión de 'tmt' por SPI al SLAVE
;*****************************************************************
SPI_Mtransmit:
		out		SPDR, tmt
		ret
;*****************************************************************
;*	Recepcion de 'rcv' por SPI del SLAVE
;*****************************************************************
SPI_Mreceive:
		in		rcv, SPDR
		ret

;*****************************************************************
;*	Espera del fin de la recepción SPI por Slave
;*****************************************************************
SPI_Wait:
		;*	Espera del fin de la recepción
		in 		tmp, SPSR
		sbrs 	tmp, SPIF
		rjmp 	SPI_Wait
		ret
