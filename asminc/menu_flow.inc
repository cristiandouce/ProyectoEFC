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
		rcall delay200ms

		rjmp	check_keys
		
keyUP:
		;se presiono key UP si no estába presionada antes seteo el flag y voy al mainloop, si si vuelvo
		sbrs	key,switchUP
		rjmp	check_keys
		cbr		key,maskUP
		sbr		fla,maskUP
		rjmp	PressUp
keyDW:
		sbrs	key,switchDW
		rjmp	check_keys
		cbr		key,maskDW
		sbr		fla,maskDW
		rjmp	PressDown
keyRT:
		sbrs	key,switchRT
		rjmp	check_keys
		cbr		key,maskRT
		sbr		fla,maskRT
		rjmp	PressRet

toLambda:
		ldi		Xh,high(Lambda<<1)
		ldi		Xl,low(Lambda<<1)
		rjmp 	ShowScreen

Lambda_Down:
Lambda_Up:
Lambda_Ret:		
slLambda:
		movw	rd1h:rd1l,lambdah:lambdal
		ldi tmp,0x0A ; dividimos por 10
		mov rd2,tmp
		rcall	div8
		ret

