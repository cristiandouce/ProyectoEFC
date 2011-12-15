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
		rcall delay200ms
		cbr		fla,maskRESET
		;leo el puerto C
		in		tmp,PINC
		;si está presionado un boton va a quedar en 0 ese bit
		sbrs	tmp,switchUP
		rjmp	pressUP
		sbrs	tmp,switchDW
		rjmp	pressDOWN
		sbrs	tmp,switchRT
		rjmp	pressRET
		;si no se presiono nada me vuelvo a fijar
		;luego de esperar por posibles rebotes
		
		rjmp	check_keys
		
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
		ldi 	tmp, 0x90
		add 	yl,tmp
		ldi 	tmp,0x01
		adc 	yh,tmp
		rjmp 	comunicateA
idle:
		ret

comunicateA:
        movw r1:r0,yh:yl;hago una copia de seguridad del valor de lambda
        
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

		rcall SendLambda
        
		SetLcdClearAtHome
		ldi		arg,'>'
		rcall 	lcd_putc
		ldi		Zh,high(sDone<<1)
		ldi		Zl,low(sDone<<1)
		rcall	LCD_Putstring

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
		ldi 	dtah,0x90
		add 	wrdl,dtah
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
		ldi 	zh,high(sBusy<<1)
		ldi 	zl,low(sBusy<<1)
		rcall 	LCD_PutString
		rcall 	SlaveSensorInit
		rcall 	PutSensorData
		;rcall   filldatos
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
		ldi 	yl,0x91
		ldi		Zh,high(datos)
		ldi		Zl,low(datos)
floop:	st 		z+,yl
		sbiw	Yh:Yl,1
		brne 	floop
		ret

		
ComunicateQ:
		SetLcdClearAtHome 		
		rcall 	getmotorcount
		movw	rBin1H:rBin1L,dtah:dta
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
		clr 	dtah
		ld 		dta,z+
		movw	rBin1H:rBin1L,dtah:dta
		ldi     Zh,high(buffer)
		ldi 	Zl,low(buffer)
		rcall	Bin2ToAsc5
		rcall 	LCD_PutAscii
		ret


