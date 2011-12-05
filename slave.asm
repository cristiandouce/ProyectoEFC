;/*
; * slave.asm
; *
; *  Created: 03/12/2011 01:34:41 p.m.
; *   Author: Susana
; */ 
 ;*****************************************************************
;*	File: slave.asm
;*	
;*	Espectrofotocolorimetro Microcontrolador SLAVE
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
 	.include "asminc/motordef.inc"
;defino constantes

; Por ahora incluidas en los respectivos archivos de include.


;********************************************************************************************************
;Defino las variables
	.dseg
		.org 0x200
		var:	.byte	120
 cant_pasos:	.byte   2
 promedio: 		.byte 	1

	.cseg
		.org 0x0000
		rjmp RESET

;*****************************************************************
;*	Defino los vectores de interrupcion
;*****************************************************************
	.org 0x011
		rjmp SPI_STC ; SPI Transfer Complete Handler

;*****************************************************************
;*	Inicialización del Micro luego del RESET
;*****************************************************************
RESET:
		;*	Inicialización del StackPointer
		ldi	tmp, low(RAMEND)
		out	SPL, tmp
		ldi	tmp, high(RAMEND)
		out	SPH, tmp
	    ;Conf posta
		rjmp MAIN



;*****************************************************************
;*	MAIN Program for microcontroller
;*****************************************************************
MAIN:	
		;* Inicio el SPI como SLAVE
		cbi DDRD, 0;configuro RX como entrada
		sbi	DDRD, 1; TX como salida
		sbi DDRD,4; XCK como salida,master mode

		;* Y ESTO??? COMMENTS PLEASE!!!
        ldi tmp,0b00001111
        out DDRC,tmp
        cbi DDRB,0
        cbi DDRB,1
        cbi DDRB,6
        cbi DDRB,6
        sbi PORTB,0
        sbi PORTB,1
        sbi PORTB,6

        ;* Posicion inicial motor
        ldi tmp,low(posicion1<<1)
		mov wrdl,tmp
		ldi tmp,high(posicion1<<1)
		mov wrdh,tmp

		;* Inicializa motor
								; Lo que conviene es lo siguiente:
								; El master cuando inicia
								; envia una instruccion al slave de iniciar motor
								; este puede devolver ok (si termina)
								; o error (si no puede o lo que sea)
								; Cuestion:
								; Mientras el motor se inicializa
								; el master muestra en pantalla
								; "Iniciando motor!!"
								; "Espere por favor..."
		rcall contarpasos


		;* Inicializo comunicacion SPI
		rcall	SPI_Sinit

		;* Inicio y configuro el Sensor
		rcall sensor_Init
		;rcall sensor_configuracion

		;* Espero instrucciones del master
		ldi	Xl,low(cant_pasos)
	    ldi	Xh,high(cant_pasos)
		sei
		
idle:
		sei
espera:
		rjmp espera

	.include "asminc/usart_slave.inc"
	.include "asminc/calibracion.inc"
	.include "asminc/common.inc"		
	.include "asminc/spi_slave.inc"
	.include "asminc/procesar_lectura.inc"
	.include "asminc/sensor.inc"
	.include "asminc/timer_slave.inc"
 	.include "asminc/motorcontrol.inc"



