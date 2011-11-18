;*****************************************************************
;*	File: usart_master.inc
;*	
;*	Rutinas de control de USART en modo SLAVE
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




;*****************************************************************
;*****************************************************************
;*				Rutinas para trabajar con USART
;*****************************************************************
;*****************************************************************


;*****************************************************************
;*	"USART_Init"
;*	Llamada a inicialización.
;*	Permite configurar las opciones del USART
;*****************************************************************

USART_Init:
	;seteo el baud rate a 9600 || TXEN true || Buffer free || U2X0 true || Character Size = 8bit
	; Sets low nibble to 0 (Baud Rate)
	ldi tmp,0x00
	sts UBRR0H,tmp
	; Sets low nibble to 12 (Baud Rate = 9600 with U2X0 = 1)
	ldi tmp,0x0C;
	sts UBRR0L,tmp
	; Sets Buffer Free and U2X0 to 1
	ldi tmp,0b00100010
	sts UCSR0A,tmp
	; Set TXEN
	ldi tmp,0b00001000
	sts UCSR0B,tmp
	; Sets Character Size to 8bit
	ldi tmp,0b00000110
	sts UCSR0C,tmp
	ret

;********************************************************************************************************
;Rutina que envia un byte por la usart

USART_Transmit:
		;Wait for empty transmit buffer
		lds	 tmp,UCSR0A
		sbrs tmp,UDRE0
		rjmp USART_Transmit
		; Put data into buffer, sends the data
		sts UDR0,arg
		ret


;********************************************************************************************************
; Set bit 4
USART_Receive:
; Wait for data to be received
	lds	 tmp,UCSR0A
	sbrs tmp,RXC0
	rjmp USART_Receive
; Get and return received data from buffer
	lds rtn, UDR0
	ret
