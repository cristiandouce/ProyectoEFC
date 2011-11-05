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

	.equ	sline		=	0xC0
	.equ	lcdclear	=	0x01
	.equ	lcdhome		=	0x02

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
			
				
			ldi		arg, lcdclear			;* Borro display, cursor -> home
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
LCD_cmd8b:	in	tmp, DDRD		;seteamos el nibble alto de DDRD sin tocar nada mas
			sbr	tmp, 0b11110000	;seteo nibble alto en temp
			out	DDRD, tmp		;reemplazo DDRD
			in	tmp, PortD		;recupero el valor del puerto
			cbr	tmp, 0b11110000	;seteo los bits de datos
			cbr	arg, 0b00001111	;y borro el nibble bajo del argumento para no sobreescribir los bits de control
			or	tmp, arg		
			out	PortD, tmp		;y escribo en el puerto.
			sbi	PortD, LCD_E		;hago un pulso en E
			nop
			nop
			nop
			cbi	PortD, LCD_E
			in	tmp, DDRD		;vuelvo a dejar el DDRD con los bits de datos en entrada
			cbr	tmp, 0b11110000
			out	DDRD, tmp		
			ret
			
;*****************************************************************
;*	"LCD_putc"
;*	Comando de 4 bits para pasar caracter
;*****************************************************************
LCD_putc:	push 	arg 				;guardo el argumento
			in		tmp, DDRD			;guardo los bits de address de datos
			sbr		tmp, 0b11110000		;seteo los bits de datos en salida
			out		DDRD, tmp			
			in		tmp, PortD			;recupero los datos de PortD
			cbr		tmp, 0b11111110		;borro todas las lineas de LCD
			cbr		arg, 0b00001111		;escondo el nibble bajo del argumento
			or		tmp, arg			;seteo los bits del argumento en temp
			out		PortD, tmp			;y escribo el valor del puerto
			sbi		PortC, LCD_RS		;seteo RS
			sbi		PortD, LCD_E		;hago un pulso en E
			nop
			nop
			nop
			cbi		PortD, LCD_E
			pop		arg					;recupero el argumento para usar el nibble bajo
			cbr		tmp, 0b11110000		;borro los bits de address de datos
			swap	arg					;swapeo para poner el bit bajo en el alto
			cbr		arg, 0b00001111		;borro los bits no usados en el argumento
			or		tmp, arg			;y los seteo en el puerto
			out		PortD, tmp			
			sbi		PortC, LCD_RS		;vuelvo a setear RS
			sbi		PortD, LCD_E		;y poner un pulso en E
			nop
			nop
			nop
			cbi		PortD, LCD_E
			cbi		PortC, LCD_RS
			in		tmp, DDRD
			cbr		tmp, 0b11110000		;las lineas de datos vuelven a ser salida
			out		DDRD, tmp
			ret

;*****************************************************************
;*	"LCD_putc"
;*	Comando de 4 bits para mandar instrucción al LCD
;*	mismo que LCD_putchar, pero con RS bajo
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
;*	funciona como LCD_getchar, pero con RS en 0, rtn.7 es la flag busy
;*****************************************************************
LCD_geta:	in		tmp, DDRD
			andi 	tmp, 0b00001111		
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

