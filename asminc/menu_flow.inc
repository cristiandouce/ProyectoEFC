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

lVerCal:
		ldi		Xh,high(VerCal<<1)
		ldi		Xl,low(VerCal<<1)
		rjmp	ShowScreen

lProbarP:
		ldi		Xh,high(ProbarP<<1)
		ldi		Xl,low(ProbarP<<1)
		rjmp	ShowScreen

lResCal:
		ldi		Xh,high(ResCal<<1)
		ldi		Xl,low(ResCal<<1)
		rjmp	ShowScreen

lFakeCal:
		ldi		Xh,high(FakeCal<<1)
		ldi		Xl,low(FakeCal<<1)
		rjmp	ShowScreen

check_keys:
		;reseteo las flags de las keys
		rcall delay200ms
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
		rcall StartReading
		rcall SaveReading
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
		ldi		Xh,high(MotorM<<1)
		ldi		Xl,low(MotorM<<1)
		rjmp 	ShowScreen


		
ComunicateL:
		SetLcdClearAtHome 		
		ldi 	zh,high(sBusy<<1)
		ldi 	zl,low(sBusy<<1)
		rcall 	LCD_PutString
		rcall 	StartReading
		rcall 	SaveReading
		clr 	yh
		clr 	yl
		ldi		Xh,high(Resultados<<1)
		ldi		Xl,low(Resultados<<1)
		rjmp 	ShowScreen

ComunicateP:
		SetLcdClearAtHome 		
		ldi 	zh,high(sBusy<<1)
		ldi 	zl,low(sBusy<<1)
		rcall 	LCD_PutString
		rcall 	getPixeles
		rcall 	SavePixeles
		clr 	yh
		clr 	yl
		ldi		Xh,high(Pixeles<<1)
		ldi		Xl,low(Pixeles<<1)
		rjmp 	ShowScreen
Pixel_Down:
		sbiw 	YH:YL,1
		brmi 	set100
		rjmp   	ShowScreen

set100:
		clr 	Yh
		ldi 	Yl,0x64
		rjmp 	ShowScreen

Pixel_Up:
		clr 	Zh
		ldi 	Zl,0x64
		adiw 	YH:YL,1
		cp 	 	Yh,Zh
		cpc	 	Yl,Zl
		in  tmp,SREG
        sbrc tmp,1 ;tests zero flag
        rjmp Pixel_up
		rjmp   	ShowScreen



slPixel:
		secondline
		movw	rBin1H:rBin1L,yh:yl
		ldi     Zh,high(buffer)
		ldi 	Zl,low(buffer)
		rcall	Bin2ToAsc5
		rcall 	LCD_PutAscii
		ret

ComunicateV:
		SetLCDClearAtHome
		rcall 	geteprom
		putstring sEstado
		ldi 	Yh,high(datoscalibracion)
		ldi 	Yl,low(datoscalibracion)
		SECONDLINE
		clr 	dtah
		ld 		dta,Y+
		PrintAscii  dtah,dta,0
		rcall 	delay1s
		rcall 	delay1s
		SetLCDClearAtHome
		ld 		dtah,Y+
		ld 		dta,Y+
		PrintAscii  dtah,dta,1
		SECONDLINE
		ld 		dtah,Y+
		ld 		dta,Y+
		PrintAscii  dtah,dta,2
		rcall 	delay1s
		rcall 	delay1s
		SetLCDClearAtHome
		ld 		dtah,Y+
		ld 		dta,Y+
		PrintAscii  dtah,dta,3
		SECONDLINE
		ld 		dtah,Y+
		ld 		dta,Y+
		PrintAscii  dtah,dta,4
		rcall 	delay1s
		rcall 	delay1s
		SetLCDClearAtHome
		ld 		dtah,Y+
		ld 		dta,Y+
		PrintAscii  dtah,dta,5
		SECONDLINE
		ld 		dtah,Y+
		ld 		dta,Y+
		PrintAscii  dtah,dta,6
		rcall 	delay1s
		rcall 	delay1s
		rjmp ShowScreen

ComunicateR:
		SetLCDClearAtHome
		rcall 	setEepromReset

		ldi		arg,'>'
		rcall 	lcd_putc
		ldi		Zh,high(sDone<<1)
		ldi		Zl,low(sDone<<1)
		rcall	LCD_Putstring
		rcall delay1s
		rjmp ShowScreen

ComunicateF:
		SetLCDClearAtHome
		rcall 	setEepromFake
		
		ldi		arg,'>'
		rcall 	lcd_putc
		ldi		Zh,high(sDone<<1)
		ldi		Zl,low(sDone<<1)
		rcall	LCD_Putstring
		rcall delay1s
		rjmp ShowScreen

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
 		

dispintensidad:
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


