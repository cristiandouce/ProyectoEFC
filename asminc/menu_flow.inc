mainloop:
		mov		tmp,fla
		andi	tmp,maskMN
		cpi		tmp,menu1
		breq	main_menu
		cpi		tmp,menu2
		breq	export_menu
		cpi		tmp,menu3
		breq	calib_menu
		cpi		tmp,menu4
		breq	res_menu
		rjmp	error



;***********************************************************************
;*	Chequeo si se presiono alguna tecla y seteo el flag correspondiente
;***********************************************************************
check_keys:
		;reseto las flags de las keys
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



;***********************************************************************
;*	Menu inicial(por defecto entra aqui)
;***********************************************************************
main_menu:
		mov		tmp,fla
		andi	tmp,maskOP
		cpi		tmp,op1
		breq	stepLec
		cpi		tmp,op2
		breq	stepCal
		cpi		tmp,op3
		breq	stepLmp
		rjmp	error

;**************************************************************************
;*	Menu Exportar
;**************************************************************************
export_menu:
		mov		tmp,fla
		andi	tmp,maskOP
		cpi		tmp,op1
		breq	stepExpSi
		cpi		tmp,op2
		breq	stepExpNo
		rjmp	error
;**************************************************************************
;*	Menu Calibrar
;**************************************************************************
calib_menu:
		mov		tmp,fla
		andi	tmp,maskOP
		cpi		tmp,op1
		breq	stepCal2Si
		cpi		tmp,op2
		breq	stepCal2No
		cpi		tmp,op3
		breq	stepCal2Back
		cpi		tmp,op4
		breq	stepCal1
		rjmp	error
;**************************************************************************
;*	Menu Mostrar resultados
;**************************************************************************
res_menu:


;**************************************************************************
;*	Acciones en menues
;**************************************************************************

stepCal:
		;si flagUP:display_lec
		sbrc	fla,flagUP
		rjmp	display_lec
		;si flagDW:display_Lmp
		sbrc	fla,flagDW
		rjmp	display_Lmp
		;si flagRT; FN_Cal
		sbrc	fla,flagRT
		rjmp	FN_Cal
		;si noflag:no deberia pasar
		rjmp	error
		


stepLmp:
		;si flagUP:display_Cal
		sbrc	fla,flagUP
		rjmp	display_Cal
		;si flagDW:nada
		sbrc	fla,flagDW
		rjmp	check_keys
		;si flagRT; FN_Lmp
		sbrc	fla,flagRT
		rjmp	FN_Lmp
		;si noflag:no deberia pasar
		rjmp	error

stepExpSi:
		sbrc	fla,flagUP
		rjmp	display_ExpNo
		;si flagDW:paso de si a no
		sbrc	fla,flagDW
		rjmp	display_ExpNo
		;si flagRT; exporto
		sbrc	fla,flagRT
		rjmp	FN_export
		rjmp	error
stepExpNo:
		sbrc	fla,flagUP
		rjmp	display_ExpSi
		;si flagDW:display_Cal
		sbrc	fla,flagDW
		rjmp	display_ExpSi
		;si flagRT; FN_Lec
		sbrc	fla,flagRT
		rjmp	FN_Backtomain
		rjmp	error
stepLec:
		;si flagUP:nada
		sbrc	fla,flagUP
		rjmp	check_keys
		;si flagDW:display_Cal
		sbrc	fla,flagDW
		rjmp	display_Cal
		;si flagRT; FN_Lec
		sbrc	fla,flagRT
		rjmp	FN_Lec
		;si noflag:display_lec
		rjmp	display_Lec
stepCal2SI:
		;si flagUP:display_Cal2Back
		sbrc	fla,flagUP
		rjmp	display_Cal2Back
		;si flagDW:display_Cal2No
		sbrc	fla,flagDW
		rjmp	display_Cal2No
		;si flagRT; FN_calibrar
		sbrc	fla,flagRT
		rjmp	FN_calibrar
		rjmp	error
stepCal2No:
		sbrc	fla,flagUP
		rjmp	display_Cal2Si
		;si flagDW:display_Cal
		sbrc	fla,flagDW
		rjmp	display_Cal2Back
		;si flagRT; 
		sbrc	fla,flagRT
		rjmp	FN_Cal
		rjmp	error
stepCal2Back:
		sbrc	fla,flagUP
		rjmp	display_Cal2No
		;si flagDW:display_Cal
		sbrc	fla,flagDW
		rjmp	display_Cal2Si
		;si flagRT; FN_Lec
		sbrc	fla,flagRT
		rjmp	FN_Backtomain
		rjmp	error



stepCal1:
		;si flagUP:subirdigito
		sbrc	fla,flagUP
		rjmp	display_Cal1up
		;si flagDW:nada
		sbrc	fla,flagDW
		rjmp	display_Cal1down
		;si flagRT; FN_Lmp
		sbrc	fla,flagRT
		rjmp	display_Cal1confirm
		;si noflag:no deberia pasar
		rjmp	error




display_Cal1up:
		rcall	initiatedisplaycal1
		inc		aux
		cpi		aux,0x0A
		brge	largoarriba
regresodelargoarriba:
		cpi		con,0x01
		breq	dispdigito1
		cpi		con,0x02
		breq	dispdigito2
		cpi		con,0x03
		breq	dispdigito3
		rjmp	error

largoarriba:
		ldi		aux,0x00
		rjmp	regresodelargoarriba
	
display_Cal1down:
		rcall	initiatedisplaycal1
		dec		aux
		cpi		aux,0xFF
		breq	largoabajo
regresodelargoabajo:
		cpi		con,0x01
		breq	dispdigito1
		cpi		con,0x02
		breq	dispdigito2
		cpi		con,0x03
		breq	dispdigito3
		rjmp	error		

largoabajo:
		ldi		aux,0x09
		rjmp	regresodelargoabajo

display_Cal1confirm:
		rcall	initiatedisplaycal1
		inc		con
		cpi		con,0x01
		breq	start_digito1
		cpi		con,0x02
		breq	start_digito2
		cpi		con,0x03
		breq	start_digito3
		cpi		con,0x04
		breq	sigpaso
		rjmp	error



start_digito1:
		ldi		aux,0x00
		rjmp	dispdigito1
		
start_digito2:		
		mov		lmd,aux
		ldi		aux,0x00
		rjmp	dispdigito2

start_digito3:
		swap	aux
		or		lmd,aux
		ldi		aux,0x00
		rjmp	dispdigito3

		
dispdigito1:
		mov		arg,aux
		rcall	BCDtoLCD
		rjmp	check_keys

dispdigito2:
		mov		arg,lmd
		rcall	BCDtoLCD
		mov		arg,aux
		rcall	BCDtoLCD
		rjmp	check_keys

dispdigito3:
		mov		arg,lmd
		ldi		tmp,0x0F
		and		arg,tmp
		rcall	BCDtoLCD
		mov		arg,lmd
		ldi		tmp,0xF0
		and		arg,tmp
		swap	arg
		rcall	BCDtoLCD
		mov		arg,aux
		rcall	BCDtoLCD
		rjmp	check_keys
sigpaso:
		ubicar	op1,maskOP
		rjmp	display_Cal2Si
		

;*Seleccionada Lectura
display_Lec:
		ubicar	op1,maskOP
		SetLcdClearAtHome
		;cargo primerlinea(seleccionada)
		ldi		arg,'>'
		rcall 	lcd_putc

		LoadstringZ strLec
		rcall	LCD_Putstring

		;cargo segunda linea(no seleccionada)
		
		Secondline

		; Calibro con linea superior
		ldi		arg,0x20
		rcall 	lcd_putc

		LoadstringZ strCal
		rcall	LCD_Putstring
		rjmp	check_keys

display_Cal:
		ubicar	op2,maskOP
		SetLcdClearAtHome
		;cargo primerlinea(seleccionada)
		ldi		arg,'>'
		rcall 	lcd_putc
		LoadstringZ strCal
		rcall	LCD_Putstring
		;cargo segunda linea(no seleccionada)
		Secondline
		ldi		arg,' '
		rcall 	lcd_putc		
		sbrs	fla,flagLmp
		rcall	loadlmpoff
		sbrc	fla,flagLmp
		rcall	loadlmpon

		rcall	LCD_Putstring
		rjmp	check_keys
display_Lmp:
		ubicar	op3,maskOP
		;vacio pantalla
		SetLcdClearAtHome
		;cargo primer linea(no seleccionada)
		ldi		arg,' '
		rcall 	lcd_putc
		LoadstringZ strCal
		rcall	LCD_Putstring
		;Cargo segunda linea(seleccionada)
		Secondline
		ldi		arg,'>'
		rcall 	lcd_putc
		sbrs	fla,flagLmp
		rcall	loadlmpoff
		sbrc	fla,flagLmp
		rcall	loadlmpon
		rcall	LCD_Putstring
		rjmp	check_keys
display_ExpNo:
		ubicar	op2,maskOP
		SetLcdClearAtHome
		;cargo primerlinea(seleccionada)
		LoadstringZ strExp
		rcall	LCD_Putstring
		;Cargo segunda linea

		Secondline
		LoadstringZ strSi
		rcall	LCD_Putstring
		ldi		arg,'/'
		rcall 	lcd_putc
		ldi		arg,'*'
		rcall 	lcd_putc		
		LoadstringZ strNo
		rcall	LCD_Putstring
		ldi		arg,'*'
		rcall 	lcd_putc
		rjmp	check_keys
display_ExpSi:
		ubicar	op1,maskOP

		SetLcdClearAtHome

		;cargo primerlinea(seleccionada)
		LoadstringZ strExp
		rcall	LCD_Putstring
		;Cargo segunda linea

		Secondline
		ldi		arg,'*'
		rcall 	lcd_putc
		LoadstringZ strSi
		rcall	LCD_Putstring
		ldi		arg,'*'
		rcall 	lcd_putc
		ldi		arg,'/'
		rcall 	lcd_putc
		LoadstringZ strNo
		rcall	LCD_Putstring
		rjmp	check_keys

display_Cal2Si:
		ubicar	op1,maskOP

		SetLcdClearAtHome

		;cargo primerlinea(seleccionada)
		LoadstringZ strCL
		rcall	LCD_Putstring
		;Cargo segunda linea

		Secondline
		ldi		arg,'*'
		rcall 	lcd_putc
		LoadstringZ strSi
		rcall	LCD_Putstring
		ldi		arg,'*'
		rcall 	lcd_putc
		ldi		arg,'/'
		rcall 	lcd_putc
		LoadstringZ strNo
		rcall	LCD_Putstring
		ldi		arg,'/'
		rcall 	lcd_putc
		LoadstringZ strBck
		rcall	LCD_Putstring
		rjmp	check_keys



display_Cal2No:
		ubicar	op2,maskOP

		SetLcdClearAtHome

		;cargo primerlinea(seleccionada)
		LoadstringZ strCL
		rcall	LCD_Putstring
		;Cargo segunda linea

		Secondline
		LoadstringZ strSi
		rcall	LCD_Putstring
		ldi		arg,'/'
		rcall 	lcd_putc
		ldi		arg,'*'
		rcall 	lcd_putc
		LoadstringZ strNo
		rcall	LCD_Putstring
		ldi		arg,'*'
		rcall 	lcd_putc
		ldi		arg,'/'
		rcall 	lcd_putc
		LoadstringZ strBck
		rcall	LCD_Putstring
		rjmp	check_keys



display_Cal2Back:
		ubicar	op3,maskOP

		SetLcdClearAtHome

		;cargo primerlinea(seleccionada)
		LoadstringZ strCL
		rcall	LCD_Putstring
		;Cargo segunda linea

		Secondline
		LoadstringZ strSi
		rcall	LCD_Putstring
		ldi		arg,'/'
		rcall 	lcd_putc
		LoadstringZ strNo
		rcall	LCD_Putstring
		ldi		arg,'/'
		rcall 	lcd_putc
		ldi		arg,'*'
		rcall 	lcd_putc
		LoadstringZ strBck
		rcall	LCD_Putstring
		ldi		arg,'*'
		rcall 	lcd_putc
		rjmp	check_keys


FN_calibrar:
		ubicar	menu1,maskMN
		ubicar	op1,maskOP
		SetLcdClearAtHome
		LoadstringZ strSi
		rcall	LCD_Putstring
		rcall	delay1s
		rjmp	display_Lec
FN_export:
		ubicar	menu1,maskMN
		ubicar	op1,maskOP
		SetLcdClearAtHome
		LoadstringZ strDone
		rcall	LCD_Putstring
		rcall	delay1s
		rjmp	display_Lec
FN_Lec:
		SetLcdClearAtHome
		LoadstringZ strBsy
		rcall	LCD_Putstring
		ldi		arg,'.'
		rcall	lcd_putc
		rcall	delay1s
		ldi		arg,'.'
		rcall	lcd_putc
		rcall	delay1s
		ldi		arg,'.'
		rcall	lcd_putc
		rcall	delay1s
		SetLcdClearAtHome
		LoadstringZ strBsy
		ubicar	menu2,maskMN
		
		;* ACA SOLO MUESTRO QUE SE COMUNICA CON SLAVE Y RECIBO DATA
		;* Ordeno el inicio del sensor
		rcall 	SlaveSensorInit
	
		;* Busco los datos de la lectura
		rcall 	PutSensorData
		rcall delay1s


		rjmp	display_ExpSi


FN_Backtomain:
		ubicar	menu1,maskMN
		ubicar	op1,maskOP
		SetLcdClearAtHome
		rjmp	display_Lec

FN_Cal:
		ldi		con,0x00
		ubicar	op4,maskOP
		ubicar	menu3,maskMN
		rjmp	display_Cal1confirm
FN_Lmp:
		sbrs	fla,flagLmp
		sbi		PORTC,lamp		
		sbrc	fla,flagLmp
		cbi		PORTC,lamp
		ldi		tmp,maskLmp
		eor		fla,tmp	
		rjmp	display_lmp
