;*****************************************************************
;*	File: usart_master.inc
;*	
;*	Rutinas de control de USART en modo MASTER
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
	ldi tmp,0b00011000
	sts UCSR0B,tmp
	; Sets Character Size to 8bit (2:1)
	; and Parity Even (5:4)
	; and 2 stop bits (3)
	ldi tmp,0b00101110
	sts UCSR0C,tmp
	ret

;********************************************************************************************************
;Rutina que envia un byte por la usart

USART_Transmit:
		push tmp
saltito:
		;Wait for empty transmit buffer
		lds	 tmp,UCSR0A
		sbrs tmp,UDRE0
		rjmp saltito
		; Put data into buffer, sends the data
		sts UDR0,arg
		
		; Temporary Check Received Data equal
		; to sended one
		; rcall USART_Receive
		; cp rtn,arg
		; brne saltito
		; End temporary

		pop tmp
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
