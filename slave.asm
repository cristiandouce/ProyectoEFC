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
 	.include "asminc/macros_slave.inc"
;defino constantes

; Por ahora incluidas en los respectivos archivos de include.


;********************************************************************************************************
;Defino las variables
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
		.org 0x200
		
pos_max:    .byte 2
delta_paso:    .byte 2
delta_lambda:  .byte 2
var:	.byte	120
cant_pasos:	.byte   2
promedio: 		.byte 	1
mediciones:     .byte 400
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
		;seteo el prescaler para que divida por 4 la frec del cristal
		ldi tmp,0x80
		sts $0061,tmp
		ldi tmp,0x02 
		sts $0061,tmp
		;* Inicio el SPI como SLAVE
		cbi DDRD, 0;configuro RX como entrada
		sbi	DDRD, 1; TX como salida
		sbi DDRD,4; XCK como salida,master mode
		


		;* Y ESTO??? COMMENTS PLEASE!!!
        ldi tmp,0b00001111
        out DDRC,tmp
        cbi DDRB,0
        cbi DDRB,1
        cbi DDRC,5
        sbi PORTB,0
        sbi PORTB,1
        sbi PORTC,5

		sbi DDRC,4 ;configuro SlaveStatus como salida
        sbi PORTC,4

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
	.include "asminc/sensor.inc"
	.include "asminc/timer_slave.inc"
 	.include "asminc/motorcontrol.inc"
 	.include "asminc/eeprom.inc"





