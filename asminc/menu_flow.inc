;*****************************************************************
;*	File: menu_flow.inc
;*	
;*	Este archivo contiene el flujo de menu y llamados
;*	desde el microcontrolador de ROL master
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
;*****************************************************************



ShowScreen:
		SetLcdClearAtHome
		movw	Zh:Zl,Xh:Xl
		MoveZTo (firstlinehandler)
		rcall	jumper
		movw	Zh:Zl,Xh:Xl
		MoveZTo (secondlinehandler)
		rcall   jumper
		rjmp	check_keys

flstd:
		ldi		arg,'>'
		rcall 	lcd_putc
		movw	Zh:Zl,Xh:Xl
		MoveZTo (fstline)
		LoadWord
		rcall	LCD_Putstring
		ret
slstd:
		Secondline
		ldi		arg,' '
		rcall 	lcd_putc		
		movw	Zh:Zl,Xh:Xl
		MoveZTo (scdline)
		LoadWord
		rcall	LCD_Putstring
		ret

flend:
		ldi		arg,' '
		rcall 	lcd_putc
		movw	Zh:Zl,Xh:Xl
		MoveZTo (fstline)
		LoadWord
		rcall	LCD_Putstring
		ret

slLamp2:
		Secondline
		ldi		arg,'>'
		rcall 	lcd_putc
		rjmp	siglamp
slLamp:
		Secondline
		ldi		arg,' '
		rcall 	lcd_putc
siglamp:
		ldi		Zh,high(sLampOn<<1)
		ldi		Zl,low(sLampOn<<1)
		sbis 	PORTC,0
		MoveZTo (16);es sLampOn-sLampOff
		rcall	LCD_Putstring
		ret

jumper:
		LoadWord
		ijmp

setandshow:;revisar
		movw	Yh:Yl,Xh:Xl
		rjmp 	ShowScreen
	

PressUp:
		movw	Zh:Zl,Xh:Xl
		rjmp	jumper
PressDown:
		movw	Zh:Zl,Xh:Xl
		MoveZTo	(next)
		rjmp	jumper
PressRet:
		movw	Zh:Zl,Xh:Xl
		MoveZTo	(fwd)
		rjmp	jumper


switchLamp:
		sbis 	PORTC,0
		rjmp	switchLamp1	
		cbi		PORTC,lamp
		rjmp	ShowScreen		
switchLamp1:
		sbi 	PORTC,lamp
		rjmp 	ShowScreen

lCalibra:
		ldi		Xh,high(Calibra<<1)
		ldi		Xl,low(Calibra<<1)
		rjmp 	ShowScreen

lLectura:
		ldi		Xh,high(Lectura<<1)
		ldi		Xl,low(Lectura<<1)
		rjmp	ShowScreen
lProbar:
		ldi		Xh,high(Probar<<1)
		ldi		Xl,low(Probar<<1)
		rjmp	ShowScreen
lProbarM:
		ldi		Xh,high(ProbarM<<1)
		ldi		Xl,low(ProbarM<<1)
		rjmp	ShowScreen

lLamp:
		ldi		Xh,high(Lampara<<1)
		ldi		Xl,low(Lampara<<1)
		rjmp	ShowScreen

check_keys:
		;reseteo las flags de las keys
		rcall delayX
		cbr		fla,maskRESET
		;leo el puerto C
		in		tmp,PINC
		;si está presionado un boton va a quedar en 0 ese bit
		sbrs	tmp,switchUP
		rjmp	keyUP
		sbrs	tmp,switchDW
		rjmp	keyDW
		sbrs	tmp,switchRT
		rjmp	keyRT
		sbrc	tmp,switchUP
		sbr		key,maskUP
		sbrc	tmp,switchDW
		sbr		key,maskDW
		sbrc	tmp,switchRT
		sbr		key,maskRT
		;si no se presiono nada me vuelvo a fijar
		;luego de esperar por posibles rebotes
		
		rjmp	check_keys
		
keyUP:
		;se presiono key UP si no estaba presionada antes seteo el flag y voy al mainloop, si si vuelvo
		;sbrs	key,switchUP
		;rjmp	check_keys
		cbr		key,maskUP
		sbr		fla,maskUP
		rjmp	PressUp
keyDW:
		;sbrs	key,switchDW
		;rjmp	check_keys
		cbr		key,maskDW
		sbr		fla,maskDW
		rjmp	PressDown
keyRT:
		;sbrs	key,switchRT
		;rjmp	check_keys
		cbr		key,maskRT
		sbr		fla,maskRT
		rjmp	PressRet

toLambda:
		clr 	yh
		clr 	yl
		ldi		Xh,high(Lambda<<1)
		ldi		Xl,low(Lambda<<1)
		rjmp 	ShowScreen

Lambda_Down:
		sbiw 	YH:YL,1
		brmi 	set400
		rjmp   	ShowScreen

set400:
		ldi 	Yh,0x1
		ldi 	Yl,0x90
		rjmp 	ShowScreen

Lambda_Up:
		ldi 	Zh,0x01
		ldi 	Zl,0x91
		adiw 	YH:YL,1
		cp 	 	Yh,Zh
		cpc	 	Yl,Zl
		breq 	set0
		rjmp   	ShowScreen
set0:
		clr 	Yh
		clr 	Yl
		rjmp 	ShowScreen

Lambda_Ret:
		;rjmp 	comunicateC
		rjmp 	comunicateA
idle:
		ret

comunicateA:
        mov r1,wrdh;hago una copia de seguridad del valor de lambda
        mov r0,wrdl
        
        ;Muestro el valor seleccionado de lambda nuevamente en el display

		movw	rBin1H:rBin1L,r1:r0
		ldi     Zh,high(buffer)
		ldi 	Zl,low(buffer)
		rcall	Bin2ToAsc5
		ldi tmp,5
		ld	arg,Z+
tsloop10:	
		rcall USART_Transmit
		rcall LCD_Putc
		ld	arg,Z+
		dec tmp
		cpi tmp,0
		brne tsloop10
		rcall delay1s
		rcall delay1s
 
		      
        ldi arg,'c'
		rcall USART_Transmit
		ldi arg,'a'
		rcall USART_Transmit
		ldi arg,'l'
		rcall USART_Transmit
		ldi arg,'i'
		rcall USART_Transmit
		ldi arg,'b'
		rcall USART_Transmit
		ldi arg,'r'
		rcall USART_Transmit
		ldi arg,'a'
		rcall USART_Transmit
		ldi arg,'d'
		rcall USART_Transmit
		ldi arg,'o'
		rcall USART_Transmit
		ldi arg,'!'
		rcall USART_Transmit
		ldi arg,'!'
		rcall USART_Transmit
		ldi arg,'!'
		rcall USART_Transmit


		rcall SendLambda
        ;rcall PutEepromData
       
		

		SetLcdClearAtHome
		ldi		arg,'>'
		rcall 	lcd_putc
		ldi		Zh,high(sDone<<1)
		ldi		Zl,low(sDone<<1)

		rcall	LCD_Putstring

		rcall delay1s
		rcall delay1s

		

		rcall delay1s

		ldi		Xh,high(Calibra<<1)
		ldi		Xl,low(Calibra<<1)
		rjmp 	ShowScreen

comunicateC:		
		SetLcdClearAtHome
		rcall SlaveSensorInit
		rcall PutSensorData
		ldi		Zh,high(sEnviandoPC<<1)
		ldi		Zl,low(sEnviandoPC<<1)
		rcall	LCD_Putstring
        MoveCalibraTo 4
        ldi arg,'m'
        rcall EEPROM_write
        MoveCalibraTo 5
        ldi arg,'a'
        rcall EEPROM_write
        MoveCalibraTo 6
        ldi arg,'y'
       	rcall EEPROM_write
        MoveCalibraTo 6
       	rcall EEPROM_read
        mov arg,rtn
        rcall USART_Transmit


        MoveCalibraTo 5
        rcall EEPROM_read
        mov arg,rtn
        rcall USART_Transmit
        
        MoveCalibraTo 4
        rcall EEPROM_read
        mov arg,rtn
        rcall USART_Transmit

		ldi arg,'a'
		rcall USART_Transmit
		ldi arg,'h'
		rcall USART_Transmit
		ldi arg,'o'
		rcall USART_Transmit
		ldi arg,'r'
		rcall USART_Transmit
		ldi arg,'a'
		rcall USART_Transmit
		ldi arg,'!'
		rcall USART_Transmit
		SetLcdClearAtHome
		ldi		arg,'>'
		rcall 	lcd_putc
		ldi		Zh,high(sDone<<1)
		ldi		Zl,low(sDone<<1)

		rcall	LCD_Putstring
		rcall delay1s
		rcall delay1s

		ldi		Xh,high(Calibra<<1)
    	ldi		Xl,low(Calibra<<1)
		rjmp 	ShowScreen

slLambda:
		secondline
		movw	wrdh:wrdl,yh:yl
		ldi 	zh,high(sLo<<1)
		ldi 	zl,low(sLo<<1)
		rcall 	LCD_PutString
		ldi 	dta,0x1
		ldi 	dtal,0x90
		add 	wrdl,dtal
		adc 	wrdh,dta
		movw	rBin1H:rBin1L,wrdh:wrdl
		ldi     Zh,high(buffer)
		ldi 	Zl,low(buffer)
		rcall	Bin2ToAsc5
		rcall 	LCD_PutAscii
		ret

comunicateM:
		rcall 	SlaveMotorInit
		ldi		Xh,high(MotorM<<1)
		ldi		Xl,low(MotorM<<1)
		rjmp 	ShowScreen


		
ComunicateL:
		SetLcdClearAtHome 		
		;rcall 	SlaveSensorInit
		;rcall 	PutSensorData
		rcall   filldatos
		;rcall	SlavePrepMotorC
		;rcall 	getmotorcount
		clr 	yh
		clr 	yl
		ldi		Xh,high(Resultados<<1)
		ldi		Xl,low(Resultados<<1)
		rjmp 	ShowScreen
		ret

filldatos:
		ldi 	yh,0x1
		ldi 	yl,0x90
		ldi		Zh,high(datos)
		ldi		Zl,low(datos)
floop:	st 		z+,yh
		st 		z+,yl
		sbiw	Yh:Yl,1
		brne 	floop
		ret

		
ComunicateQ:
		SetLcdClearAtHome 		
		rcall 	getmotorcount
		movw	rBin1H:rBin1L,dtal:dta
		ldi     Zh,high(buffer)
		ldi 	Zl,low(buffer)
		rcall	Bin2ToAsc5
		ldi tmp,5
		ld	arg,Z+
tsloop7:	
		rcall USART_Transmit
		rcall LCD_Putc
		ld	arg,Z+
		dec tmp
		cpi tmp,0
		brne tsloop7
		rcall delay1s
		rcall delay1s
		rjmp 	ShowScreen
;			
;res_up:
 ;   	push xh
 ;   	push xl
 ;   	adiw Yh:Yl,1
;		ldi Zh,high(datos)
;		ldi Zl,low(datos)
;		movw xh:xl,Yh:Yl
;		sub xl,Zl
;		sbc xh,Zh
;		ldi Zh,high(cant_pasos)
;		ldi Zl,low(cant_pasos)
;		ld wrdh,Z+
;		ld wrdl,Z+
;		movw Zh:zl,wrdh:wrdl
;		cp  Zl,xl
;		cpc Zh,xh;
;;		brlo OVERFLOWDATA;
;;    	pop xl;
;    	pop xh;
;		rjmp 	ShowScreen;
;;
;OVERFLOWDATA:;
;		pop xl;
;		pop xh
;		ldi Yh,high(datos)
;		ldi Yl,low(datos)
;		rjmp ShowScreen
;
;res_down:
;    	ldi 	tmp,low(datos)
;    	cpi	 	Yh,high(datos)
;    	cpc 	Yl,tmp
;    	breq 	UNDERFLOWDATA
;		sbiw	Yh:Yl,1
;    	rjmp ShowScreen

	
;UNDERFLOWDATA:
;		ldi Zh,high(cant_pasos)
;		ldi Zl,low(cant_pasos)
;		ld wrdh,Z+
;		ld wrdl,Z+
;		movw Zh:Zl,wrdh:wrdl
;		ldi Yh,high(datos)
;		ldi Yl,low(datos)
;		add Yl,Zl
;		adc Yh,Zh
;		rjmp ShowScreen
		

dispintensidad:
		;secondline
		ldi 	zh,high(sInt<<1)
		ldi 	zl,low(sInt<<1)
		rcall 	LCD_PutString
		ldi 	zh,high(datos)
		ldi 	zl,low(datos)
		add 	zl,yl
		adc 	zh,yh
		add 	zl,yl
		adc 	zh,yh
		ld 		dtal,z+
		ld 		dta,z+
		movw	rBin1H:rBin1L,dtal:dta
		ldi     Zh,high(buffer)
		ldi 	Zl,low(buffer)
		rcall	Bin2ToAsc5
		rcall 	LCD_PutAscii
		ret



;*****************************************************************

;*****************************************************************
;*****************************************************************
;*				Rutinas para trabajar con EEPROM
;*****************************************************************
;*****************************************************************

;*****************************************************************
;*	"EEPROM_write"
;*	Escribe en memoria EEPROM.
;*	Recibe en r25:r24 la direccion sobre la cual escribir
;* 	y guarda el valor transportado dentro de 'arg'
;*****************************************************************

EEPROM_write:
		; Wait for completion of previous write
		sbic EECR,EEPE 
		rjmp EEPROM_write ; Set up address (r25:r24) in address register 
		out EEARH, r25 
		out EEARL, r24 ; Write data (arg) to Data Register 
		out EEDR,arg ; Write logical one to EEMPE 
		sbi EECR,EEMPE ; Start eeprom write by setting EEPE 
		sbi EECR,EEPE 
		ret

;*****************************************************************
;*	"EEPROM_read"
;*	Escribe en memoria EEPROM.
;*	Recive en r25:r24 la direccion de la cual leer
;* 	y lee el valor en memoria devolviendolo en 'rtn'
;*****************************************************************

EEPROM_read:
		; Wait for completion of previous write
		sbic EECR,EEPE 
		rjmp EEPROM_read ; Set up address (r25:r24) in address register 
		out EEARH, r25 
		out EEARL, r24 ; Start eeprom read by writing EERE 
		sbi EECR,EERE ; Read data from Data Register 
		in rtn,EEDR 
		ret