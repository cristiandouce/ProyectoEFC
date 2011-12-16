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
	.equ	pSS	=	2
	.equ	pMOSI	=	3
	.equ	pMISO	=	4
	.equ	pSCK	=	5
	.equ	cnf_slv =	0b11100000
    .equ    estado  =  0
    .equ    lambda1High = 1
    .equ    lambda1Low = 2
    .equ    calibra1High = 3
    .equ    calibra1Low = 4
    .equ    lambda2High = 5
    .equ    lambda2Low = 6
    .equ    calibra2High =7
    .equ    calibra2Low = 8
    .equ    deltaPasoXlambdaHigh=9
    .equ    deltaPasoXlambdaLow=10
    .equ    posMinLambdaHigh=11
    .equ    posMinLambdaLow=12 
    .equ    velmotor=0x04
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
		
            ;* Guardo el byte recibido
		in dta,SPDR
		
    		;* Me fijo si voy a recibir Lambda
        cpi tmp2,0
        in  tmp,SREG
        sbrs tmp,1 ;tests zero flag
        rjmp savelambda

            ;* Si recibo null, no hago nada!
        cpi dta,0
        in  tmp,SREG
        sbrc tmp,1 ;tests zero flag
        rjmp return_interrupt
        
            ;* Test de eeprom Read
        cpi dta,'e'
        in  tmp,SREG
        sbrc tmp,1 ;tests zero flag
        rjmp eepromReadTest

        cpi dta,'m'
        in  tmp,SREG
        sbrc tmp,1 ;tests zero flag
        rjmp initmotor

        cpi dta,'t'
        in  tmp,SREG
        sbrc tmp,1 ;tests zero flag
        rjmp takepicture


        cpi dta,'f'
        in  tmp,SREG
        sbrc tmp,1 ;tests zero flag
        rjmp partealta

        ;* Me fijo que instrucción se envió...

	cpi dta,'s'           ;me fijo si fue la orden 's'
	in  tmp,SREG
        sbrc tmp,1 ;tests zero flag
        rjmp ReadSensor       ;y voy a hacer la rutina dl sensor

	cpi dta,'d'
        in  tmp,SREG
        sbrc tmp,1 ;tests zero flag
        rjmp SendSensorData 
        	
	cpi dta,'p'
	in  tmp,SREG
        sbrc tmp,1 ;tests zero flag
        rjmp PrepMotorCount

	cpi dta,'c'
	in  tmp,SREG
        sbrc tmp,1 ;tests zero flag
        rjmp SendMotorCount

	cpi dta,'u'
	in  tmp,SREG
        sbrc tmp,1 ;tests zero flag
        rjmp MotorUp

	cpi dta,'j'
	in  tmp,SREG
        sbrc tmp,1 ;tests zero flag
        rjmp MotorDown

            ;* Hago la calibracion
	cpi dta,'a'
	in  tmp,SREG
        sbrc tmp,1 ;tests zero flag
        rjmp preparacalibrar

        cpi dta,'b'
        in  tmp,SREG
        sbrc tmp,1 ;tests zero flag
        rjmp fakeeeprom 

        cpi dta,'o'
        in  tmp,SREG
        sbrc tmp,1 ;tests zero flag
        rjmp prepeeprom 

        cpi dta,'l'
        in  tmp,SREG
        sbrc tmp,1 ;tests zero flag
        rjmp sendeeprom 

        cpi dta,'r'
        in  tmp,SREG
        sbrc tmp,1 ;tests zero flag
        rjmp reseteeprom 

return_interrupt:
		cbi PortC,4
		reti

eepromReadTest:
        mov     tmp,lambdal
        out     SPDR,tmp
        rjmp    return_interrupt

partealta:
        mov tmp,lambdah
		out SPDR,tmp
		rjmp	return_interrupt
	
savelambda:
		cpi tmp2,1
		breq savelambdal
		cpi tmp2,3
		breq savelambdah
        dec tmp2
		rjmp return_interrupt

savelambdah:
        mov lambdah,dta
        dec tmp2
	rjmp return_interrupt

takepicture:
        rcall   sensor_reset
        rcall   sensor
        ldi     Xl,low(var);vuelvo a hacer q apunte al comienzo
        ldi     Xh,high(var)
        rjmp    return_interrupt


savelambdal:
        mov lambdal,dta
        dec tmp2
		;rjmp return_interrupt
		rjmp calibrar

ReadSensor:
        rcall   lectura
		ldi	Xl,low(mediciones);vuelvo a hacer q apunte al comienzo
	        ldi	Xh,high(mediciones)
		rjmp	return_interrupt

MotorUp:
		rcall stepback
		rjmp return_interrupt
MotorDown:
		rcall stepforth
		rjmp return_interrupt

SendSensorData:
		ld tmp,X+
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
		ldi 	tmp2,4
		rjmp return_interrupt;sino, no tengo nada q calibrar

calibrar:
        MoveCalibraTo estado;veo el estado d la calibracion 
        rcall EEPROM_read 
        mov tmp,rtn
        cpi tmp,0 	;significa q antes calibre el segundo
        breq primera_calibracion
        cpi tmp,1;significa q hice la primera calibracion,hago la segunda
        breq segunda_calibracion
        rjmp return_interrupt;sino, no tengo nada q calibrar

primera_calibracion:
         MoveCalibraTo lambda1High
         mov arg,lambdah
         rcall EEPROM_write
         
         MoveCalibraTo lambda1Low
         mov arg,lambdal
         rcall EEPROM_write        

        rcall calibracion
        ;aca pedir ayuda dl macro,x ahora lo hago a mano
        
        MoveCalibraTo calibra1High        
        ldi Xh,high(pos_max)
        ldi Xl,low(pos_max)
        ld tmp,X+
        mov arg,tmp
        rcall EEPROM_write
        MoveCalibraTo calibra1Low
        ld tmp,X
        mov arg,tmp
        rcall EEPROM_write


        MoveCalibraTo estado
        ldi arg,1
        rcall EEPROM_write
        rcall conversion
        rjmp return_interrupt

segunda_calibracion:
        
        MoveCalibraTo lambda2High
        mov arg,lambdah
        rcall EEPROM_write
         
        MoveCalibraTo lambda2Low
        mov arg,lambdal
        rcall EEPROM_write   

        rcall calibracion
        
        ldi Xh,high(pos_max)
        ldi Xl,low(pos_max)
        
        MoveCalibraTo calibra2High        
        ld tmp,X+
        mov arg,tmp
        rcall EEPROM_write

        MoveCalibraTo calibra2Low  
        ld tmp,X
        mov arg,tmp
        rcall EEPROM_write

        MoveCalibraTo estado
        ldi arg,0
        rcall EEPROM_write
        rcall conversion		;una vez q tengo las dos calibraciones,
        						;busco la equivalencia entre pasos y longitudes
        						;de onda
        rjmp return_interrupt   
 

prepeeprom:
        clr yh
        clr yl
        rjmp return_interrupt

sendeeprom:
        rcall eeprom_read
        inc Yl
        out SPDR,rtn
        rjmp return_interrupt

reseteeprom:
        MoveCalibraTo estado
        ldi tmp,13
loopReset:
        ldi arg,0x00
        rcall EEPROM_write
        inc Yl
        dec tmp
        brne loopReset
        rjmp return_interrupt


fakeeeprom:
        
        MoveCalibraTo deltaPasoXlambdaHigh
        ldi arg,0x00
        rcall EEPROM_write
         
        MoveCalibraTo deltaPasoXlambdaLow
        ldi arg,0x03
        rcall EEPROM_write

        MoveCalibraTo posMinLambdaHigh
        ldi arg,0x01
        rcall EEPROM_write
         
        MoveCalibraTo posMinLambdaLow
        ldi arg,0x00
        rcall EEPROM_write 

        MoveCalibraTo calibra1High
        ldi arg,0x02
        rcall EEPROM_write
         
        MoveCalibraTo calibra1Low
        ldi arg,0x8A
        rcall EEPROM_write

        MoveCalibraTo calibra2High
        ldi arg,0x01
        rcall EEPROM_write
         
        MoveCalibraTo calibra2Low
        ldi arg,0x2C
        rcall EEPROM_write 

        MoveCalibraTo lambda1High
        ldi arg,0x02
        rcall EEPROM_write
         
        MoveCalibraTo lambda1Low
        ldi arg,0xBC
        rcall EEPROM_write

        MoveCalibraTo lambda2High
        ldi arg,0x01
        rcall EEPROM_write
         
        MoveCalibraTo lambda2Low
        ldi arg,0xF4
        rcall EEPROM_write 
        rjmp return_interrupt

initmotor:
        ;* Posicion inicial motor
        ldi tmp,low(posicion1<<1)
        mov wrdl,tmp
        ldi tmp,high(posicion1<<1)
        mov wrdh,tmp
        rcall contarpasos
        rjmp return_interrupt