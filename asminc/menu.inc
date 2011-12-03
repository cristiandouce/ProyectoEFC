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
;*****************************************************************
;*				Rutinas para trabajar con el MENU
;*****************************************************************
;*****************************************************************

MENU_init:
		;*inicializo Flags
		ldi 	Yh,0x00
		ldi 	Yl,0x01
		ldi tmp,1
		ldi key,0xff
		ldi fla,0x00
		;seteo posición inicial del menu
		ldi		Xh,high(Lectura<<1)
		ldi		Xl,low(Lectura<<1)
		;seteo los switches como entrada con pull up on
		cbi		DDRC,switchUP
		cbi		DDRC,switchDW
		cbi		DDRC,switchRT
		sbi		PORTC,switchUP
		sbi		PORTC,switchDW
		sbi		PORTC,switchRT
		;seteo pin lampara como salida en 0
		sbi		DDRC,lamp
		cbi		PORTC,lamp
		;cargo mi registro ZERO
		ldi 	tmp,0
		mov		zero,tmp
		ret


initiatedisplaycal1:
		SetLcdClearAtHome
		;LoadstringZ strLmd
		rcall	LCD_Putstring
		Secondline
		ret

	
LCD_Putstring:
		;mov aux,chq
		lpm	arg,Z+
psloop:	
		rcall lcd_putc
		lpm	arg,Z+
		;dec aux
		;cpi aux,0
		cpi arg,fst
		brne psloop
		ret

LCD_PutAscii:
		ldi tmp,5
		ld	arg,Z+
paloop:	
		rcall lcd_putc
		ld	arg,Z+
		dec tmp
		cpi tmp,0
		;cpi arg,fst
		brne paloop
		ret

BCDtoLCD:
		ldi	tmp,0x30
		add	arg,tmp
		rcall	lcd_putc
		ret
		
		

error:
		SetLcdClearAtHome
		;LoadstringZ strError
		rcall	LCD_Putstring
