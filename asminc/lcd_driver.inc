;*****************************************************************
;*	File: lcd_driver.inc
;*	
;*	Driver para LCD con 4 (cuatro) bit de datos
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
	.equ	LCD_RS	= 1
	.equ	LCD_RW	= 2
	.equ	LCD_E	= 3

;*****************************************************************
;*****************************************************************
;*				Rutinas para trabajar con el LCD
;*****************************************************************
;*****************************************************************

;*****************************************************************
;*	"LCD_init"
;*	Llamada a inicialización.
;*	Permite configurar las opciones del LCD
;*****************************************************************
LCD_init:	sbi		DDRC,LCD_RS
			sbi		DDRD,LCD_RW
			sbi		DDRD,LCD_E
			rcall	delay200ms			;* Esperamos que inicie el lcd


			ldi		arg, 0x20			;* Le decimos al lcd que queremos usarlo en modo 4-bit
			rcall	LCD_cmd8b			;* Ingreso de comando mientras aun esta en 8-bit
			rcall	LCD_wait


			ldi		arg, 0x28			;* Seteo: 2 lineas, fuente 5*7
			rcall	LCD_cmd4b
			rcall	LCD_wait


			ldi		arg, 0x0C			;* Display on, cursor off
			rcall	LCD_cmd4b
			rcall	LCD_wait
			
				
			ldi		arg, 0x01			;* Borro display, cursor -> home
			rcall	LCD_cmd4b
			rcall	LCD_wait


			ldi		arg, 0x06			;* Cursor en auto-inc 
			rcall	LCD_cmd4b
			rcall	LCD_wait			;* Espero la busy flag
			ret

;*****************************************************************
;*	"LCD_cmd8b"
;*	Comando de 8 bits para pasar a modo 4 bits
;*	Usado solo en inicialización del LCD
;*****************************************************************
LCD_cmd8b:	in		tmp, DDRD			;we need to set the high nibble of DDRD while leaving
		
			;the other bits untouched. Using tmp for that.
			;set high nibble in tmp
			sbr		tmp, 0b11110000
			out		DDRD, tmp			;write value to DDRD again-> d7-d4 son salidas
			in		tmp, PortD			;then get the port value, aparentemente portd esta en cero x defecto
			cbr		tmp, 0b11110000		;borro the data bits 
			cbr		arg, 0b00001111		;then clear the low nibble of the argument
										;so that no control line bits are overwritten
			or		tmp, arg			;then set the data bits (from the argument) in the
										;Port value
										;*en argument tengo el argumento con el nibble d las variables d control en cero
										;*en tmp tngo los valores dl puerto d, con la parte d datos borrada
			out		PortD, tmp			;and write the port value.
			sbi		PortD, LCD_E		;now strobe E
			nop
			nop
			nop
			cbi		PortD, LCD_E
			in		tmp, DDRD			;get DDRD to make the data lines input again
			cbr		tmp, 0b11110000		;clear data line direction bits
			out		DDRD, tmp			;and write to DDRD
			ret

;*****************************************************************
;*	"LCD_putc"
;*	Comando de 4 bits para pasar caracter
;*****************************************************************
LCD_putc:	push 	arg 				;save the argmuent (it's destroyed in between)
			in		tmp, DDRD			;get data direction bits
			sbr		tmp, 0b11110000		;set the data lines to output
			out		DDRD, tmp			;write value to DDRD
			in		tmp, PortD			;then get the data from PortD
			cbr		tmp, 0b11111110		;clear ALL LCD lines (data and control!)
			cbr		arg, 0b00001111		;we have to write the high nibble of our argument first
										;so mask off the low nibble
			or		tmp, arg			;now set the argument bits in the Port value
			out		PortD, tmp			;and write the port value
			sbi		PortC, LCD_RS		;now take RS high for LCD char data register access
			sbi		PortD, LCD_E		;strobe Enable
			nop
			nop
			nop
			cbi		PortD, LCD_E
			pop		arg					;restore the argument, we need the low nibble now...
			cbr		tmp, 0b11110000		;clear the data bits of our port value
			swap	arg					;we want to write the LOW nibble of the argument to
										;the LCD data lines, which are the HIGH port nibble!
			cbr		arg, 0b00001111		;clear unused bits in argument
			or		tmp, arg			;and set the required argument bits in the port value
			out		PortD, tmp			;write data to port
			sbi		PortC, LCD_RS		;again, set RS
			sbi		PortD, LCD_E		;strobe Enable
			nop
			nop
			nop
			cbi		PortD, LCD_E
			cbi		PortC, LCD_RS
			in		tmp, DDRD
			cbr		tmp, 0b11110000		;data lines are input again
			out		DDRD, tmp
			ret

;*****************************************************************
;*	"LCD_putc"
;*	Comando de 4 bits para mandar instrucción al LCD
;*	Same as LCD_putc, but with RS low!
;*****************************************************************
LCD_cmd4b:	push	arg					;guardo la instruccion
			in		tmp, DDRD			;copio en tmp 00001110
			sbr		tmp, 0b11110000		; hago un or, entonces pongo en 1 el nibble alto
			out		DDRD, tmp			;pongo este valor en DDRD x lo tanto d7-d4 son salidas
			in		tmp, PortD			;guardo el valor dl puerto
			cbr		tmp, 0b11111110		;hago una and con 01,borro todos los bits
			cbr		arg, 0b00001111		;hago una and con F0, borro los 4 bits d control
			or		tmp, arg			;hago una or me quedan los 4 bits altos d la instruccion y cero debajo
			out		PortD, tmp			; lo cargo en el puerto
			sbi		PortD, LCD_E
			nop
			nop
			nop
			cbi		PortD, LCD_E
			pop		arg					;saco el argumento d la pila
			cbr		tmp, 0b11110000		; hago una and con 0F, borro los dl nibble alto dejo los d control en cero
			swap	arg					;intercambio el nibble alto con el bajo
			cbr		arg, 0b00001111		;hago una and con F0,borro el nibble inferior d argument
			or		tmp, arg			;hago or con tmp
			out		PortD, tmp			;cargo en el puerto 
			sbi		PortD, LCD_E
			nop
			nop
			nop
			cbi		PortD, LCD_E
			in		tmp, DDRD
			cbr		tmp, 0b11110000		; vuelvo a poner el nibble alto en cero para q sea entrada.
			out		DDRD, tmp
			ret

;*****************************************************************
;*	"LCD_geta"
;*	Obtiene la dirección de memoria del LCD
;*	works just like LCD_getchar, but with RS low, return.7 is the busy flag
;*****************************************************************
LCD_geta:	in		tmp, DDRD
			andi 	tmp, 0b00001111		;esto es al pedo
			out		DDRD, tmp
			cbi		PortC, LCD_RS		;pongo en cero para decir q es instruccion
			sbi		PortD, LCD_RW		; en uno para leer
			sbi		PortD, LCD_E		; seteo enable
			nop
			in		tmp, PinD			;guardo lo q estaba en el pin
			andi	tmp, 0b11110000		;borro la parte d las variables d control
			mov		rtn, tmp			;pongo en return ese valor
			cbi		PortD, LCD_E		;pongo enable en cero
			nop
			nop		
			sbi		PortD, LCD_E		;pongo enable en uno
			nop
			in		tmp, PinD			; guardo lo q esta en el pin, q es nibble bajo d la direccion, en realidad no lo necesito
			andi	tmp, 0b11110000		;borro la parte d las variables d control
			swap	tmp					; pongo el nibble alto en el bajo
			or		rtn, tmp			;hago un or con return,tngo la dire completa
			cbi		PortD, LCD_E		;pongo enable en cero
			cbi		PortD, LCD_RW		; pongo rw en cero
			ret

;*****************************************************************
;*	"LCD_wait"
;*	Rutina de BUSY FLAG
;*	Lee la dirección y la busy flag hasta que este queda en low
;*****************************************************************
LCD_wait:	rcall	LCD_geta
			andi	rtn, 0x80			;hago un and con 10000000
			brne	LCD_wait			;sale d aca solo cuando el busy flag es cero
			ret

