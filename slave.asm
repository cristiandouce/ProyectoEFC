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

;defino constantes

; Por ahora incluidas en los respectivos archivos de include.


;********************************************************************************************************
;Defino las variables
 .dseg
 .org 0x200
 ahora:	.byte	6
 var:	.byte	102


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

		; Configuracion de prueba
		ldi	Xl,low(ahora);x apunta a var
	    ldi	Xh,high(ahora)

		ldi tmp,'a'
		st X+,tmp
		ldi tmp,'h'
		st X+,tmp
		ldi tmp,'o'
		st X+,tmp
		ldi tmp,'r'
		st X+,tmp
		ldi tmp,'a'
		st X+,tmp

		ldi	Xl,low(ahora);x apunta a var
	    ldi	Xh,high(ahora)

	    ; Configuracion POSTA
		cbi DDRD, 0;configuro RX como entrada
		sbi	DDRD, 1; TX como salida
		sbi DDRD, 4; XCK como salida,master mode


		rjmp MAIN



;*****************************************************************
;*	MAIN Program for microcontroller
;*****************************************************************
MAIN:	
		; Inicializo SPI
		rcall	SPI_Sinit

		; Inicizlizo y configuro SENSOR
		rcall PROCESS_INITIALIZATION

		; Espero llamada del master
		rjmp   idle	
		
idle:
		sei
jm:
		rjmp jm

	.include "asminc/timer_slave.inc"
	.include "asminc/usart_slave.inc"
	.include "asminc/common.inc"



; ***********************************
; Functiones temporales del sensorrrr
SENSOR_FAKE:
		ldi tmp,'w'
		out	SPDR,tmp

		;rcall sensor
		ldi		Xl,low(ahora);vuelvo a hacer q apunte al comienzo
	    ldi		Xh,high(ahora)
		rjmp	return_interrupt

SENSOR_Data:
		ldi 	tmp,'k'
		out 	SPDR,tmp
		rjmp	return_interrupt	
;FIN	

	.include "asminc/spi_slave.inc"
	.include "asminc/sensor_prev.inc"
