;*****************************************************************
;*	File: master.asm
;*	
;*	Espectrofotocolorimetro Microcontrolador MASTER
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
	.include "asminc/m88def.inc"
	.include "asminc/regs_alias.inc"
;defino constantes

; Por ahora incluidas en los respectivos archivos de include.


;defino macros
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
	

	.dseg
		var:	.byte	6

	.cseg
	.org 0

		rjmp 	RESET
	

;*****************************************************************
;*	Inicialización del Micro luego del RESET
;*****************************************************************
RESET:
		ldi		tmp, low(RAMEND)
		out		SPL, tmp
		ldi		tmp, high(RAMEND)
		out		SPH, tmp
		rjmp 	MAIN


;*****************************************************************
;*	MAIN Program for microcontroller
;*****************************************************************
MAIN:
		
		;*	Habilito el LCD
		rcall	LCD_init

		;*Inicio el SPI como Master
		rcall 	SPI_Minit
		
		;* Ordeno el inicio del sensor
		rcall 	SlaveSensorInit
	
		;* Busco los datos de la lectura
		rcall 	PutSensorData

FIN_P:	rjmp 	FIN_P


SlaveSensorInit:
		;*	Mando al slave para que inicie la lectura del sensor
		SendInstruction 's'

		;*	Espero 1 segundo hasta que termine la lectura
		rcall 	delay1s
		ret

PutSensorData:
		;* cargo un contador para los 5 datos
		ldi 	delay,5
start:
		;* Mando instruccion de lectura
		;* y levanto el dato
		SendInstruction 'd'
		
		;* Cargo el dato en argumento para llevar al lcd
		mov		arg,rcv
		rcall	LCD_putc
		rcall	LCD_Wait

		;* Decremento el contador
		dec 	delay
		brne 	start
		ret

	.include "asminc/spi_master.inc"
	.include "asminc/lcd_driver.inc"
	.include "asminc/common.inc"
	
