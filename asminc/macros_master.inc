;*****************************************************************
;*	File: macros_master.inc
;*	
;*	Macros para el microcontrolador en modo MASTER
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
;*****************************************************************
;*				Macros para trabajar con MASTER
;*****************************************************************
;*****************************************************************

	.MACRO	SPI_START;*	Elijo el SLAVE con ~SS (PortB,2) en LOW
		cbi PORTB, pSS
	.ENDMACRO

	.MACRO	SPI_STOP;*	Elijo el SLAVE con ~SS (PortB,2) en LOW
		sbi PORTB, pSS
	.ENDMACRO

	.MACRO	SendInstruction
		;* Cargo la instruccion a enviar en 'tmt'
		ldi tmt,@0

		;Transmito instruccion y espero
		SPI_START
		rcall SPI_Mtransmit
		rcall SPI_Wait
		SPI_STOP

		;* Cargo NULL para no afectar Slave
		ldi tmt,0

		;* Transmito, espero y Recibo del buffer
		SPI_START
		rcall SPI_Mtransmit
		rcall SPI_Wait
		rcall SPI_Mreceive
		SPI_STOP
	.ENDMACRO
