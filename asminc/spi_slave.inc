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
		;sbi	DDRB,pCSS
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
		sbi PortC,4
		in dta,SPDR;guardo el byte q me mando
		;* Me fijo si voy a recibir Lambda
        cpi fla,0
        brne savelambdah

        ;* Me fijo que instrucción se envió...

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

		cpi dta,'a';hago la calibracion
		breq preparacalibrar

return_interrupt:
		cbi PortC,4
		reti

savelambdah:
		cpi fla,1
		breq savelambdal
		mov lambdah,dta
		dec fla
		rjmp return_interrupt

savelambdal:
		mov lambdal,dta
		dec fla
		rjmp calibrar

ReadSensor:
		ldi tmp,'w'
		out	SPDR,tmp

		rcall sensor
		rcall sensor_reset; PARA Q LOS PIXELS VUELVAN A CERO
		ldi	Xl,low(promedio);vuelvo a hacer q apunte al comienzo
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
		ldi arg,4
		rjmp	return_interrupt	

SendMotorCount:
		ld tmp,X+
		out SPDR,tmp
		rjmp	return_interrupt	
preparacalibrar:
		ldi 	fla,2
		rjmp return_interrupt;sino, no tengo nada q calibrar

calibrar:
        clr Yh
        clr Yl
        MoveCalibraTo 0;veo el estado d la calibracion 
        rcall EEPROM_read 
        mov tmp,rtn
        cpi tmp,0 	;significa q antes calibre el segundo
        breq primera_calibracion
        cpi tmp,1;significa q hice la primera calibracion,hago la segunda
        breq segunda_calibracion
        rjmp return_interrupt;sino, no tengo nada q calibrar

primera_calibracion:
        rcall calibracion
        ;aca pedir ayuda dl macro,x ahora lo hago a mano
        clr Yh
        clr Yl
        MoveCalibraTo 3        
        ldi Xh,high(pos_max)
        ldi Xl,low(pos_max)
        ld tmp,X+
        mov arg,tmp
        rcall EEPROM_write
        MoveCalibraTo 4
        ld tmp,X
        mov arg,tmp
        rcall EEPROM_write


        MoveCalibraTo 0
        ldi arg,1
        rcall EEPROM_write
        rcall conversion
        rjmp return_interrupt

segunda_calibracion:
        rcall calibracion
        ;aca pedir ayuda dl macro,x ahora lo hago a mano
        clr Yh
        clr Yl
        MoveCalibraTo 7        
        ldi Xh,high(pos_max)
        ldi Xl,low(pos_max)
        ld tmp,X+
        mov arg,tmp
        rcall EEPROM_write
        MoveCalibraTo 8  
        ld tmp,X
        mov arg,tmp
        rcall EEPROM_write
        MoveCalibraTo 0
        ldi arg,0
        rcall EEPROM_write
        rcall conversion		;una vez q tengo las dos calibraciones,
        						;busco la equivalencia entre pasos y longitudes
        						;de onda
        rjmp return_interrupt          