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
		breq ReadSensor;y voy a hacer la rutina dl sensor
		cpi dta,'d'
		breq SendSensorData		
		cpi dta,'p'
		breq PrepMotorCount
		cpi dta,'c'
		breq SendMotorCount
		cpi dta,'u'
		breq MotorUp
		cpi dta,'j'
		breq MotorDown
		cpi dta,'m'
		breq return_interrupt


return_interrupt:
		reti

ReadSensor:
		ldi tmp,'w'
		out	SPDR,tmp

		rcall sensor
		rcall sensor_reset; PARA Q LOS PIXELS VUELVAN A CERO
		rcall procesar_lectura
		ldi	Xl,low(promedio)			;vuelvo a hacer q apunte al comienzo
	    ldi	Xh,high(promedio)
		rjmp	return_interrupt

MotorUp:
		rcall stepback
		rjmp return_interrupt
MotorDown:
		rcall stepforth
		rjmp return_interrupt

SendSensorData:
		ld tmp,X
		out SPDR,tmp
		rjmp	return_interrupt	

PrepMotorCount:
		ldi xh,high(cant_pasos)
		ldi xl,low(cant_pasos)
		rjmp	return_interrupt	

SendMotorCount:
		ld tmp,X+
		out SPDR,tmp
		rjmp	return_interrupt	
