;*****************************************************************
;*	File: master.asm
;*	
;*	Espectrofotocolorimetro Microcontrolador MASTER
;*	
;*	Autores:
;*			- Rodriguez Ca�ete, Macarena
;*			- Pepe, Ezequiel Ignacio
;*			- Douce Su�rez, Cristian Gabriel
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
	.eseg
		.org 0x0000
	calibraData:		;Apunta al principio de los datos
						;de calibracion
	estadoCal: 		.db 0x00
	priLambda:  	.dw 0x0000
	priPosMot:  	.dw 0x0000
	segLambda:  	.dw 0x0000
	segPosMot:  	.dw 0x0000
	deltaPaLa:  	.dw 0x0000
	lambdaMinPos:	.dw 0x0000	
	.dseg
		var:	.byte	6
	cant_pasos: .byte 	2
	datoscalibracion: .byte 13
	.cseg
	.org 0
		rjmp 	RESET
	

;*****************************************************************
;*	Inicializaci�n del Micro luego del RESET
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
		;seteo el prescaler para que divida por 4 la frec del cristal
		ldi tmp,0x80
		sts $0061,tmp
		ldi tmp,0x02 
		sts $0061,tmp

		cbi DDRC,5 ;Seteo SlaveStatus como entrada
		sbi PORTC,5
		
		;* Habilito el LCD
		rcall	LCD_init

		;* Inicio el SPI como Master
		rcall 	SPI_Minit

		;* Inicio el USART 
		rcall 	Usart_Init
		
		;*	Inicio el motor
		rcall	SlaveMotorInit
		;* Ordeno el inicio del Menu
		rcall	MENU_init

		;* Meto todo el codigo de jmps del menu.
	.include "asminc/menu_flow.inc"

FinPrograma:
		rjmp 	FinPrograma

	.include "asminc/common.inc"
	.include "asminc/ascii.inc"
	.include "asminc/spi_master.inc"
	.include "asminc/usart_master.inc"
	.include "asminc/lcd_driver.inc"
	.include "asminc/menu.inc"
	

