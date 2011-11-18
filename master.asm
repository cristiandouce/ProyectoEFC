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
	.include "asminc/menudef.inc"
	.include "asminc/regs_alias.inc"
	.include "asminc/macros_master.inc"	
		
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
		;* Habilito el LCD
		rcall	LCD_init

		;* Inicio el SPI como Master
		rcall 	SPI_Minit

		;* Inicio el USART 
		rcall 	Usart_Init
		
		;* Ordeno el inicio del Menu
		rcall	MENU_init

		;* Meto todo el codigo de jmps del menu.
	.include "asminc/menu_flow.inc"

FIN_P:
		rjmp 	FIN_P

;* esto hay que meterlo en otro inc, asi como todo el
;* codigo opciones del menu y manejarlo con includes
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
		mov		arg,tmp
		rcall	LCD_putc
		rcall	LCD_Wait

		;* Decremento el contador
		dec 	delay
		brne 	start
		ret

	.include "asminc/common.inc"
	.include "asminc/spi_master.inc"
	.include "asminc/usart_master.inc"
	.include "asminc/lcd_driver.inc"
	.include "asminc/menu.inc"
	
