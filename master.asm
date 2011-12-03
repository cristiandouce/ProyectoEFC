/*
 * master.asm
 *
 *  Created: 03/12/2011 01:27:34 p.m.
 *   Author: Papirepipedos
 */ 

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

	.include "m88def.inc"
	.include "asminc/menudef.inc"
	.include "asminc/regs_alias.inc"
	.include "asminc/macros_master.inc"	
		
	.dseg
		var:	.byte	6
	cant_pasos: .byte 	2

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

FinPrograma:
		rjmp 	FinPrograma

	.include "asminc/common.inc"
	.include "asminc/ascii.inc"
	.include "asminc/spi_master.inc"
	.include "asminc/usart_master.inc"
	.include "asminc/lcd_driver.inc"
	.include "asminc/menu.inc"
	

