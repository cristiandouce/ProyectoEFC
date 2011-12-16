;*****************************************************************
;*	File: spi_master.inc
;*	
;*	Rutinas de control de SPI en modo MASTER
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
	.equ	cnf_mst =	0b01110001



;*****************************************************************
;*	Configuración de la comunicación SPI en MASTER
;*****************************************************************
SPI_Minit:
		;*	Set de SCK, MOSI y ~SS como salidas y MISO como
		;*	entrada
		sbi		DDRB,pMOSI
		sbi		DDRB,pSCK
		sbi		DDRB,pSS
		;cbi	DDRB,pCSS
		;*	Habilita comunicación SPI como MASTER a frecuencia
		;*	de clock de f/16
		ldi 	tmp, cnf_mst
		out 	SPCR, tmp
		in 		tmp, SPSR
		in 		tmp, SPDR
		ret


;*****************************************************************
;*	Transmisión de 'tmp' por SPI al SLAVE
;*****************************************************************
SPI_Mtransmit:
		out		SPDR, tmp
		ret
;*****************************************************************
;*	Recepcion de 'tmp' por SPI del SLAVE
;*****************************************************************
SPI_Mreceive:
		in		tmp, SPDR
		ret

;*****************************************************************
;*	Espera del fin de la recepción SPI por Slave
;*****************************************************************
SPI_Wait:
		in 		tmp, SPSR
		sbrs 	tmp, SPIF
		rjmp 	SPI_Wait
		ret

SPI_WAIT_FOR_SLAVE:
		in 		aux,PINC
		andi 	aux,0b00100000
		brne	SPI_WAIT_FOR_SLAVE ;Cuando esta en cero sigo de largo
		ret
;*****************************************************************
;*	Funciones de handle de la comunicacion SPI
;*****************************************************************

;* esto hay que meterlo en otro inc, asi como todo el
;* codigo opciones del menu y manejarlo con includes
StartReading:
		;*	Mando al slave para que inicie la lectura del sensor
		SendInstruction 's'
		ret

		
SlavePrepMotorC:
		;*	Mando al slave para que inicie la lectura del sensor
		SendInstruction 'p'
		ret

SendLambda:
		;*	Mando al slave para que inicie la se prepare a leer lambda
		SendInstruction 'a'
		SendData r1;paso la parte alta de lambda 
		SendData r0; paso la parte baja de lambda
		ret

geteprom:
		sendinstruction 'o'
		ldi 	delay,13
		ldi 	Zh,high(datoscalibracion)
		ldi 	Zl,low(datoscalibracion)
loopepr:sendinstruction 'l'
		st z+,tmp
		dec delay
		brne	loopepr
		ret

SetEepromReset:
		sendinstruction 'r'
		ret

SetEepromFake:
		sendinstruction 'b'
		ret

PutEepromData:
		SetLcdClearAtHome
		;* cargo un contador para el unico dato
		ldi 	delay,1
start88:
		;* Mando instruccion de lectura
		;* y levanto el dato
		
		SendInstruction 'f'
		;* Cargo el dato en argumento para llevar al lcd
		mov		Yh,tmp

		SendInstruction 'e'
		
		;* Cargo el dato en argumento para llevar al lcd
		mov Yl,tmp
		movw	rBin1H:rBin1L,Yh:Yl
		ldi     Zh,high(buffer)
		ldi 	Zl,low(buffer)
		rcall	Bin2ToAsc5
		ld	arg,Z+
		ld	arg,Z+
		ldi tmp,3
		ld	arg,Z+
tsloop88:	
		rcall USART_Transmit
		ld	arg,Z+
		dec tmp
		cpi tmp,0
		brne tsloop88
		ldi arg,','
		rcall USART_Transmit
		;* Decremento el contador
		ret

SaveReading:
		ldi 	contH,0x1
		ldi 	ContL,0x91;de 400 a 800 inclusive
		ldi		Zh,high(datos)
		ldi		Zl,low(datos)
start:	;st 		z+,yl
		;ldi 	delay,102
		;* Mando instruccion de lectura
		;* y levanto el dato
		SendInstruction 'd'
		;* Cargo el dato en argumento para llevar al lcd
		st 		z+,tmp
		clr Yh
		mov Yl,tmp
		movw	rBin1H:rBin1L,Yh:Yl
		ldi     Zh,high(buffer)
		ldi 	Zl,low(buffer)
		rcall	Bin2ToAsc5
		ld	arg,Z+
		ld	arg,Z+
		ldi tmp,3
		ld	arg,Z+
tsloop:	
		rcall USART_Transmit
		ld	arg,Z+
		dec tmp
		cpi tmp,0
		brne tsloop
		ldi arg,','
		rcall USART_Transmit
		;* Decremento el contador
		sbiw	contH:contL,1
		brne 	start
		ret

GetMotorCount:
		;* Mando instruccion de lectura
		;* y levanto el dato
		SendInstruction 'p'
		SendInstruction 'c'
		mov dta,tmp
		SendInstruction 'c'
		mov dtah,tmp
		ret

SlaveMotorInit:
		;*	Mando al slave para que inicie la lectura del sensor
		SetLCDClearAtHome
		putstring sInimot
		SendInstruction 'm'
		ret



mot_down:
		;*	Mando al slave para que inicie la lectura del sensor
		SendInstruction 'j'
		rjmp 	ShowScreen
mot_up:
		;*	Mando al slave para que inicie la lectura del sensor
		SendInstruction 'u'
		rjmp 	ShowScreen