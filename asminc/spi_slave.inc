;*****************************************************************
;*	File: spi_slave.inc
;*	
;*	Rutinas de control de SPI en modo SLAVE
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
	.equ	cnf_slv =	0b11100000

;*****************************************************************
;*****************************************************************
;*				Rutinas para trabajar con SPI
;*****************************************************************
;*****************************************************************

;*****************************************************************
;*	Configuración de la comunicación SPI en SLAVE
;*****************************************************************
SPI_Sinit:
		;*	Set MISO output, all others input
		sbi DDRB,pMISO
		sbi	DDRB,pCSS
		;*	Habilita SPI, como SLAVE
		ldi r17,cnf_slv
		out SPCR,r17
		in	tmp,SPSR
		in	tmp,SPDR
		ret


;*****************************************************************
;*	Operaciones de las Interrupciones
;*****************************************************************
SPI_STC:
		in dta,SPDR;guardo el byte q me mando
		cpi dta,'s';me fijo si fue la orden 's'
		breq PROCESS_CCD;y voy a hacer la rutina dl sensor
		cpi dta,'d'
		breq SENSOR_Data
		cpi dta,'p';prueba de envio y recepcion en master
		breq SENSOR_FAKE
		
return_interrupt:
		reti
