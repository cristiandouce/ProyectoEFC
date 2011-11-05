

mainloop:
		movw	Zh:Zl,Xh:Xl
		lpm		id,Z
		sbrc	fla,flagUP
		rjmp  	PressUp
		sbrc	fla,flagDW
		rjmp	PressDown
		sbrc	fla,flagFW
		rjmp	PressFwd
		rjmp	check_keys
		

PressDown:
		MoveZTo	next
		lpm		Xh,Z+
		lpm		Xl,Z+
		cp      Xl,zero
		cpc 	Xh,zero
		breq	PressDownToSpecial
		movw	Zh:Zl,Xh:Xl
		lpm		id,Z
		MoveZTo	next
		lpm		wrdh,Z+
		lpm		wrdl,Z+
		;estoy en ret
		MoveZTo	(str-fwd)
		SetLcdClearAtHome
		ldi		arg,'>'
		rcall 	lcd_putc
		rcall	LCD_Putstring
		Secondline
		cp      wrdl,zero
		cpc 	wrdh,zero
		breq	SpecialSecondLine
		movw	Zh:Zl,wrdh:wrdl
		MoveZTo	(str)
		ldi		arg,' '
		rcall 	lcd_putc
		rcall	LCD_Putstring
		rjmp	check_keys

PressDownToSpecial:
	SetLcdClearAtHome
	MoveZTo (str-fwd);Z esta en fwd
	ldi		arg,' '
	rcall 	lcd_putc
	rcall	LCD_Putstring
	Secondline
	ldi		arg,'>'
	rcall 	lcd_putc
	mov tmp,id
	cpi tmp,2
	breq ATLamp
	rjmp check_keys

SpecialSecondLine:
	ldi		arg,' '
	rcall 	lcd_putc
	mov tmp,id
	cpi tmp,2
	breq LampLine
	rjmp check_keys

ATLamp:
		ldi		Zh,high(LampOn<<1)
		ldi		Zl,low(LampOn<<1)
		sbis 	PORTC,0
		MoveZTo (22);es LampOn-LampOff
		movw	Xh:Xl,Zh:Zl
		rjmp asd
LampLine:
		ldi		Zh,high(LampOn<<1)
		ldi		Zl,low(LampOn<<1)
		sbis 	PORTC,0
		MoveZTo (22);es LampOn-LampOff
asd:	MoveZTo	(str)
		rcall	LCD_Putstring
		rjmp	check_keys

PressUp:
		MoveZTo	prev
		lpm		Xh,Z+
		lpm		Xl,Z+
		cp      Xl,zero
		cpc 	Xh,zero
		breq	PressUpToSpecial
		movw	Zh:Zl,Xh:Xl
		lpm		id,Z
		MoveZTo	next
		lpm		wrdh,Z+
		lpm		wrdl,Z+
		;estoy en fwd
		MoveZTo	(str-fwd)
		SetLcdClearAtHome
		ldi		arg,'>'
		rcall 	lcd_putc
		rcall	LCD_Putstring
		SecondLine
		cp      wrdl,zero
		cpc 	wrdh,zero
		breq	SpecialSecondLine
		movw	Zh:Zl,wrdh:wrdl
		MoveZTo	(str)
		ldi		arg,' '
		rcall 	lcd_putc
		rcall	LCD_Putstring
		rjmp	check_keys

PressUpToSpecial:
	SetLcdClearAtHome
	MoveZTo (str-next);Z esta en fwd
	ldi		arg,' '
	rcall 	lcd_putc
	rcall	LCD_Putstring
	Secondline
	ldi		arg,'>'
	rcall 	lcd_putc
	mov tmp,id
	cpi tmp,1
	breq ATLamp
	rjmp check_keys


PressFwd:
	rjmp ferror





downlambda:
	rjmp ferror
uplambda:
	rjmp ferror

ferror:
		SetLcdClearAtHome
		ldi		arg,'!'
		rcall 	lcd_putc
	rjmp ferror
;***********************************************************************
;*	Chequeo si se presiono alguna tecla y seteo el flag correspondiente
;***********************************************************************
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

		rjmp	check_keys
		
keyUP:
		;se presiono key UP si no estába presionada antes seteo el flag y voy al mainloop, si si vuelvo
		sbrs	key,switchUP
		rjmp	check_keys
		cbr		key,maskUP
		sbr		fla,maskUP
		rjmp	mainloop
keyDW:
		sbrs	key,switchDW
		rjmp	check_keys
		cbr		key,maskDW
		sbr		fla,maskDW
		rjmp	mainloop
keyRT:
		sbrs	key,switchRT
		rjmp	check_keys
		cbr		key,maskRT
		sbr		fla,maskRT
		rjmp	mainloop





;
;display_Cal1up:
;		rcall	initiatedisplaycal1
;		inc		aux
;		cpi		aux,0x0A
;		brge	largoarriba
;regresodelargoarriba:
;		cpi		con,0x01
;		breq	dispdigito1
;		cpi		con,0x02
;		breq	dispdigito2
;		cpi		con,0x03
;		breq	dispdigito3
;		rjmp	error
;
;largoarriba:
;		ldi		aux,0x00
;		rjmp	regresodelargoarriba
;	
;display_Cal1down:
;		rcall	initiatedisplaycal1
;		dec		aux
;		cpi		aux,0xFF
;		breq	largoabajo
;regresodelargoabajo:
;		cpi		con,0x01
;		breq	dispdigito1
;		cpi		con,0x02
;		breq	dispdigito2
;		cpi		con,0x03
;		breq	dispdigito3
;		rjmp	error		
;
;largoabajo:
;		ldi		aux,0x09
;		rjmp	regresodelargoabajo
;
;display_Cal1confirm:
;		rcall	initiatedisplaycal1
;		inc		con
;		cpi		con,0x01
;		breq	start_digito1
;		cpi		con,0x02
;		breq	start_digito2
;		cpi		con,0x03
;		breq	start_digito3
;		cpi		con,0x04
;		breq	sigpaso
;		rjmp	error
;
;
;
;start_digito1:
;		ldi		aux,0x00
;		rjmp	dispdigito1
;		
;start_digito2:		
;		mov		lmd,aux
;		ldi		aux,0x00
;		rjmp	dispdigito2
;
;start_digito3:
;		swap	aux
;		or		lmd,aux
;		ldi		aux,0x00
;		rjmp	dispdigito3
;
;		
;dispdigito1:
;		mov		arg,aux
;		rcall	BCDtoLCD
;		rjmp	check_keys
;
;dispdigito2:
;		mov		arg,lmd
;		rcall	BCDtoLCD
;		mov		arg,aux
;		rcall	BCDtoLCD
;		rjmp	check_keys
;
;dispdigito3:
;		mov		arg,lmd
;		ldi		tmp,0x0F
;		and		arg,tmp
;		rcall	BCDtoLCD
;		mov		arg,lmd
;		ldi		tmp,0xF0
;		and		arg,tmp
;		swap	arg
;		rcall	BCDtoLCD
;		mov		arg,aux
;		rcall	BCDtoLCD
;		rjmp	check_keys
;sigpaso:
;		ubicar	op1,maskOP
;		rjmp	display_Cal2Si
;		
;
;		rcall	LCD_Putstring
;		rjmp	check_keys
;
;
;
;FN_calibrar:
;
;		SetLcdClearAtHome
;		;LoadstringZ strSi
;		rcall	LCD_Putstring
;		rcall	delay1s
;		rjmp	display_Lec
;FN_export:
;		ubicar	menu1,maskMN
;		ubicar	op1,maskOP
;		SetLcdClearAtHome
;		;LoadstringZ strDone
;		rcall	LCD_Putstring
;		rcall	delay1s
;		rjmp	display_Lec
;FN_Lec:
;		SetLcdClearAtHome
;		;LoadstringZ strBsy
;		rcall	LCD_Putstring
;		ldi		arg,'.'
;		rcall	lcd_putc
;		rcall	delay1s
;		ldi		arg,'.'
;		rcall	lcd_putc
;		rcall	delay1s
;		ldi		arg,'.'
;		rcall	lcd_putc
;		rcall	delay1s
;		SetLcdClearAtHome
;		;LoadstringZ strBsy
;		ubicar	menu2,maskMN
;		
;		;* ACA SOLO MUESTRO QUE SE COMUNICA CON SLAVE Y RECIBO DATA
;		;* Ordeno el inicio del sensor
;		rcall 	SlaveSensorInit
;	
;		;* Busco los datos de la lectura
;		rcall 	PutSensorData
;		rcall delay1s
;
;
;		rjmp	display_ExpSi
;
;
;FN_Backtomain:
;		ubicar	menu1,maskMN
;		ubicar	op1,maskOP
;		SetLcdClearAtHome
;		rjmp	display_Lec
;
;FN_Cal:
;		ldi		con,0x00
;		ubicar	op4,maskOP
;		ubicar	menu3,maskMN
;		rjmp	display_Cal1confirm
;FN_Lmp:
;		sbrs	fla,flagLmp
;		sbi		PORTC,lamp		
;		sbrc	fla,flagLmp
;		cbi		PORTC,lamp
;		ldi		tmp,maskLmp
;		eor		fla,tmp	
;		rjmp	display_lmp
;