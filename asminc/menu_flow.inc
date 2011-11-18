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

setandshow:
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
		ldi		Xh,high(Lambda<<1)
		ldi		Xl,low(Lambda<<1)
		rjmp 	ShowScreen

Lambda_Down:
		sbiw 	YH:YL,1
		brmi 	set999
		rjmp   	ShowScreen

set999:
		ldi 	Yh,0x03
		ldi 	Yl,0xe7
		rjmp 	ShowScreen

Lambda_Up:
		ldi 	Zh,0x03
		ldi 	Zl,0xe8
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
		ldi		Xh,high(BusyC<<1)
		ldi		Xl,low(BusyC<<1)
		rjmp 	ShowScreen
idle:
		ret
comunicateC:
		rcall delay1s
		ldi 	arg,'.'
		rcall 	LCD_putc
		rcall delay1s
		ldi 	arg,'.'
		rcall 	LCD_putc
		rcall delay1s
		ldi 	arg,'.'
		rcall 	LCD_putc
		rcall delay1s
		
		rcall SlaveSensorInit
		SetLcdClearAtHome
		rcall PutSensorData

		rcall delay1s
		rcall delay1s
		rcall delay1s
		rcall delay1s
		rcall delay1s

		Secondline
		ldi		arg,' '
		rcall 	lcd_putc
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

		rcall delay1s
		rcall delay1s

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
		movw	rBin1H:rBin1L,Yh:Yl
		ldi     Zh,high(buffer)
		ldi 	Zl,low(buffer)
		rcall	Bin2ToAsc5
		rcall 	LCD_PutAscii
		ret

