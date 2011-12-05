;*****************************************************************
;*	File: sensor.inc
;*	
;*	Rutinas de control de SENSOR en el SLAVE
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


	.equ sclk=4
	.equ sdin=1
	.equ sdout=0
	.equ control=PORTD

;*****************************************************************
;*****************************************************************
;*				Rutinas para trabajar con SENSOR
;*****************************************************************
;*****************************************************************

;*****************************************************************
;*	"sensor_init"
;*	Rutina que inicializa el sensor.
;*	
;*****************************************************************

sensor_init:
           rcall Usart_Init_especial
           ldi dta,0xFF
           ;le mando 16 todos unos
           rcall USART_MSPIM_Transfer
           ldi dta,0xFF
           rcall USART_MSPIM_Transfer
          
           rcall sensor_reset
           rcall sensor_reset
           rcall sensor_reset

	       ldi aux,4
contando:  ldi dta,0xFF; va a contar 30 clock
		   rcall USART_MSPIM_Transfer
		   dec aux
           brne contando
		   
		   rcall sensor_configuracion
		   rcall delay200ms
		   ret

;*****************************************************************
;*	"sensor_configuracion"
;*	Rutina que configura el sensor.
;*	Configura ganancia/offset entre otros
;*****************************************************************
sensor_configuracion:
        ldi dta,0x3E;Write to mode register command;
	    rcall USART_MSPIM_Transfer
	    ldi dta,0xFE
	    rcall USART_MSPIM_Transfer

		ldi dta,0x00;Clear mode register
		rcall USART_MSPIM_Transfer
        ldi dta,0xFE
        rcall USART_MSPIM_Transfer


;In this example, we will set the gain to 0 and the offset to 0:
        ;Left offset
		ldi dta,0x80
		rcall USART_MSPIM_Transfer
        ldi dta,0xFE
		rcall USART_MSPIM_Transfer
		
		ldi dta,0x00;
		rcall USART_MSPIM_Transfer
        ldi dta,0xFE
        rcall USART_MSPIM_Transfer


	
		;Left gain
		ldi dta,0x82 
		rcall USART_MSPIM_Transfer
		ldi dta,0xFE
		rcall USART_MSPIM_Transfer


		ldi dta,0x00;
		rcall USART_MSPIM_Transfer
        ldi dta,0xFE
        rcall USART_MSPIM_Transfer

		;Middle offset
		ldi dta,0x84
		rcall USART_MSPIM_Transfer
        ldi dta,0xFE
        rcall USART_MSPIM_Transfer



		ldi dta,0x00;
		rcall USART_MSPIM_Transfer
        ldi dta,0xFE
        rcall USART_MSPIM_Transfer
		
		;Middle gain


		ldi dta,0x86
		rcall USART_MSPIM_Transfer
        ldi dta,0xFE
        rcall USART_MSPIM_Transfer

		ldi dta,0x00;
		rcall USART_MSPIM_Transfer
        ldi dta,0xFE
        rcall USART_MSPIM_Transfer

		;Right offset

		ldi dta,0x88
		rcall USART_MSPIM_Transfer
		ldi dta,0xFE
		rcall USART_MSPIM_Transfer

		ldi dta,0x00;
		rcall USART_MSPIM_Transfer
        ldi dta,0xFE
        rcall USART_MSPIM_Transfer

		;Right gain

		ldi dta,0x8A
		rcall USART_MSPIM_Transfer
		ldi dta,0xFE


        ldi dta,0x00;
		rcall USART_MSPIM_Transfer
        ldi dta,0xFE
        rcall USART_MSPIM_Transfer
		ret

;*****************************************************************
;*	"sensor_reset"
;*	Rutina que resetea la configuracion del sensor.
;*	
;*****************************************************************
sensor_reset: ldi dta,0x36			;codigo dl reset
              rcall USART_MSPIM_Transfer
			  ldi dta,0xFE
			  rcall USART_MSPIM_Transfer	;espero 6 pulsos de clock
			  ret


;*****************************************************************
;*	"sensor"
;*	Rutina ejecuta integracion y lectura del sensor.
;*	Guarda en la variable var los 102 pixeles leidos.
;*****************************************************************
sensor:

		
		rcall STARTint 
		;Start integration
		

		ldi arg,0xFA;uso un tiempo d integracion d 2560us
		rcall timer
		;Detengo la integracion
		rcall SAMPLEinit
		

		;Start Pixel Readout
		rcall READpixel 
		
		;ahora recibo los pixeles por la usart
		ldi key,102;
		
        
;mejorar esta parte con un ret antes y llamando
; a recibir independientemente del proceso del sensor
		
		;Le digo donde guardar los datos
		ldi	Xl,LOW(var)
		ldi Xh,HIGH(var)
        
        rcall cargo_bytes
  
		rcall procesar_lectura
		ret

;*****************************************************************
;*	"relojSensor"
;*	Simula un clock para el sensor.
;*	Usa el puerto D (control) para simular clock asincronico.
;*****************************************************************

relojSensor: sbi control,sclk
             cbi control,sclk  
             sbis TIFR0,0
             rjmp relojSensor
			 cbi TIFR0,0;pongo d nuevo el flag en cero
			 ldi tmp,0x00
	         out TCCR0B,tmp;apago el timer
			 ret



;###############################

USART_Init_especial:

	ldi tmp,0x00
	sts UBRR0H,tmp
	ldi tmp,0x0C; un baud rate de 4800 fosc=1MHz 
	sts UBRR0L,tmp
	ldi tmp,0b00100000; seteo el udre0
	sts UCSR0A,tmp
	ldi tmp,0b00011000;activo receptor y emisor
	sts UCSR0B,tmp
	ldi tmp,0b11000100;modo sincronico, 8 bits, sin bit d paridad
	sts UCSR0C,tmp;con 2 solo bit d stop
	ret
;#####################################
STARTint: 
        ldi dta,0x10
		rcall USART_MSPIM_Transfer
		ldi dta,0xFE
		rcall USART_MSPIM_Transfer

		ldi dta,0xFF;pulso sclk 22 veces
		rcall USART_MSPIM_Transfer
		ldi dta,0xFF;
		rcall USART_MSPIM_Transfer
		ldi dta,0xFF
		rcall USART_MSPIM_Transfer
		ret

;#################################################
;*****************************************************************
;*	"procesar_lectura"
;*	Devuelve un solo valor de la lectura
;*	Corresponde al promedio de los pixels centrales
;*****************************************************************
procesar_lectura:
				 ldi 	Xh,high(var)
				 ldi 	Xl,low(var)
	             clr r4
	             ldi tmp,34
mitad:           ld aux,X+
				 dec tmp
				 brne mitad
				 clc
				 ld low_suma,X
				 ldi tmp,31
sumar:           ld aux,X+
				 add low_suma,aux
	             adc high_suma,r4
	             dec tmp
	             brne sumar       
	             ldi tmp, 5
divide_by_two:   lsr high_suma
	             ror low_suma
                 dec tmp
                 brne divide_by_two
                 ldi Xl,low(promedio)
                 ldi Xh,high(promedio)
                 st X,low_suma
                 ld rtn,X
                 ldi Xh,high(var)
                 ldi Xl,low(var)
                 ret

;####################################################3
SAMPLEinit:
        ldi dta,0x20
		rcall USART_MSPIM_Transfer
		ldi dta,0xFE
		rcall USART_MSPIM_Transfer

		ldi dta,0xFF;pulso el sckl 5 veces
		rcall USART_MSPIM_Transfer
		ret

;#####################################################
READpixel:		
        ldi dta,0x04
		rcall USART_MSPIM_Transfer
		ldi dta,0xFE
		rcall USART_MSPIM_Transfer


		ldi dta,0xFF
		rcall USART_MSPIM_Transfer
		ldi dta,0xFF
		rcall USART_MSPIM_Transfer
		ldi dta,0xFF
		rcall USART_MSPIM_Transfer
		ldi dta,0xFF
		rcall USART_MSPIM_Transfer;espero 38 pulsos de clock,porq sino me paso y espero 46
		ret
;############################################
USART_MSPIM_Transfer:
; Wait for empty transmit buffer
lds tmp,UCSR0A
sbrs tmp,UDRE0

rjmp USART_MSPIM_Transfer

; Put data (r16) into buffer, sends the data
sts UDR0,dta

; Wait for data to be received
USART_MSPIM_Wait_RXCn:
lds tmp,UCSR0A
sbrs tmp, RXC0

rjmp USART_MSPIM_Wait_RXCn

; Get and return received data from buffer
lds dta, UDR0
ret		
;####################################

;##################################################################
cargo_bytes:
           ldi dta,0xFF
           rcall USART_MSPIM_Transfer
           clr aux
           rcall funcion_comienzo
           cpi rtn,9
           breq cargo_bytes
           ret
  
  ;#####################################
funcion_comienzo:
          ldi tmp,9
          sec;set carry flag
          clr rtn;cantidad de rotaciones
seguir:   
          inc rtn
          ror dta
          brcc llenar;llegue al bit de start
          dec tmp
          brne seguir
fin:       
          ret          
llenar:
       rcall c_bytes
       ldi dta,0xFF
       rcall USART_MSPIM_Transfer
       rcall c_bytes
       rcall saco2
       dec key
       brne llenar
       ldi rtn,0;esto es para poder salir de cargo_bytes
       rjmp fin                            

c_bytes:
       ldi arg,8
       sub arg,rtn
       cpi arg,0
       breq pido_dato;en este caso el cero esta en el msb d dta
       clr rtn
rotation:
        ror dta
        ror aux;voy llenando
        inc rtn
        dec arg
        brne rotation
        ret       
saco2:
      ror dta
      inc rtn
      ror dta
      inc rtn
      st X+,aux
      clr aux
      ret
 pido_dato:
       ldi dta,0xFF
       rcall USART_MSPIM_Transfer
       rjmp llenar