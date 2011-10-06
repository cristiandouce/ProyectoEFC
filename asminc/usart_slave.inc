;*****************************************************************
;*	File: usart_slave.inc
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
;seteo el baud rate
;importante:cuando lo uso en modo sincronico la frecuencia en xck
;esta relacionada con el baud rate, xq por cada pulso d clock en 
;xck se transmite un bit, x lo tanto una manera d saber el periodo
;en xck es hacer 1/baudrate
;Por eso es que cuando cuento los pulsos d sclk==xck,preescalo el timer por 256
;ya que da una frecuencia parecida a la de la usart
	ldi r17,0x00
	ldi r16,0x0C; un baud rate de 4800 fosc=1MHz 
	sts UBRR0H,r17
	sts UBRR0L,r16
	ldi r18,0b00100000; seteo el udre0
	sts UCSR0A,r18
	ldi r18,0b00011000;activo receptor y emisor
	sts UCSR0B,r18
	ldi r18,0b01000110;modo sincronico, 8 bits, sin bit d paridad
	sts UCSR0C,r18;con 1 solo bit d stop
	ret

;********************************************************************************************************
;Rutina que envia un byte por la usart

USART_Transmit:
		;Wait for empty transmit buffer
		lds	 r17,UCSR0A
		sbrs r17,UDRE0
		rjmp USART_Transmit
		; Put data into buffer, sends the data
		sts UDR0,dta
		ret


;********************************************************************************************************

USART_Receive:
; Wait for data to be received
	lds	 r17,UCSR0A
	sbrs r17,RXC0
	rjmp USART_Receive
; Get and return received data from buffer
	lds aux, UDR0
	ret
