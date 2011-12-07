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
		;* Cargo la instruccion a enviar en 'tmp'
		ldi tmp,@0

		SpiLogic

	.ENDMACRO

	.MACRO	SendData
		;* Cargo la instruccion a enviar en 'tmp'
		mov tmp,@0
        
		SpiLogic

	.ENDMACRO

	.MACRO SpiLogic
		;Transmito instruccion y espero
		SPI_START
		rcall SPI_Mtransmit
		rcall SPI_Wait
		SPI_STOP

		;* Cargo NULL para no afectar Slave
		ldi tmp,0

		;* Transmito, espero y Recibo del buffer
		SPI_START
		rcall SPI_Mtransmit
		rcall SPI_Wait
		rcall SPI_Mreceive
		SPI_STOP

		;SPI Wait for slave to end
		rcall SPI_WAIT_FOR_SLAVE
	.ENDMACRO


	.MACRO	LoadstringZ 		;*	Cargo string en Z
		ldi		Zh,high(@0<<1)
		ldi		Zl,low(@0<<1)
	.ENDMACRO
	
	.MACRO	SetLcdClearAtHome
		ldi		arg,lcdclear
		rcall	lcd_cmd4b
		rcall	LCD_wait	
		ldi		arg,lcdhome
		rcall	lcd_cmd4b
		rcall	LCD_wait
	.ENDMACRO
	
	.MACRO	LoadWord
		lpm		wrdh,Z+
		lpm		wrdl,Z+
		movw	Zh:Zl,wrdh:wrdl
	.ENDMACRO

	.MACRO	Secondline
		ldi		arg,sline
		rcall	LCD_cmd4b
		rcall	LCD_wait
	.ENDMACRO

;el siguiente macro tiene que ser de 1 sola linea
	.MACRO	MoveZTo 
		adiw	ZH:ZL,@0
	.ENDMACRO
	
	.MACRO MoveCalibraTo
        clr r25             ;por el org 000 esperemos que funque asi
        clr r24            
		adiw 	r25:r24,@0
	.ENDMACRO