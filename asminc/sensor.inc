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
           cbi control,sclk
	       cbi control,sdin
		   ldi tmp,0x04
	       out TCCR0B,tmp;divido la frecuencia por 256
		   ldi tmp,0xE2
	       out TCNT0,tmp; pongo el timer en -30
	       rcall relojSensor
		   ldi tmp,0x04
	       out TCCR0B,tmp;divido la frecuencia por 256
		   ldi tmp,0xF6;pongo el timer en -10
		   out TCNT0,tmp
		   sbi control,sdin
		   rcall relojSensor
		   rcall USART_Init;de ahora en mas no uso mas el clock manual sino el d la usart
		   rcall sensor_reset
		   ldi arg,0xFB; va a contar 5 clock
		   rcall timer
		   ;IMPORTANTE en la datasheet dice 1F,mientras q en el informe 5F,cualquier cosa se cambia
		   ldi dta,0x1F;Write to mode register command;
		   rcall send
		   ldi dta,0x00;Clear mode register
		   rcall send
		   ret

;*****************************************************************
;*	"sensor_configuracion"
;*	Rutina que configura el sensor.
;*	Configura ganancia/offset entre otros
;*****************************************************************
sensor_configuracion:
;In this example, we will set the gain to 0 and the offset to 0:
        ;Left offset
		ldi dta,0x40
		rcall send
		ldi dta,0x00;
		rcall send
		;Left gain
		ldi dta,0x41 
		rcall send
		ldi dta,0x00
		rcall send
		;Middle offset
		ldi dta,0x42
		rcall send
		ldi dta,0x00;
		rcall send
		;Middle gain
		ldi dta,0x43
		rcall send
		ldi dta,0x00
		rcall send
		;Right offset
		ldi dta,0x44
		rcall send
		ldi dta,0x00
		rcall send
		;Right gain
		ldi dta,0x45
		rcall send
        ldi dta,0x00
        rcall send
		ret

;*****************************************************************
;*	"sensor_reset"
;*	Rutina que resetea la configuracion del sensor.
;*	
;*****************************************************************
sensor_reset: ldi dta,0x1B			;codigo dl reset
              rcall USART_Transmit
			  ldi arg,0xFA			;espero 6 pulsos de clock
			  rcall timer
			  ret


;*****************************************************************
;*	"sensor"
;*	Rutina ejecuta integracion y lectura del sensor.
;*	Guarda en la variable var los 102 pixeles leidos.
;*****************************************************************
sensor:

		ldi arg,0xE2
		rcall timer;hago un delay d como minimo un milisegundo
		;Start integration
		ldi dta,0x08
		rcall send
		ldi arg,0xEA;pulso sclk 22 veces
		rcall timer

		ldi arg,0xF6;uso un tiempo d integracion d 2560us
		rcall timer
		;Detengo la integracion
		ldi dta,0x10
		rcall send
		ldi arg,0xFB;pulso el sckl 5 veces
		rcall timer

		;Start Pixel Readout
		;To initiate readout from the TSL3301, send the Start Read command and wait for a start bit on SDOUT:
		ldi dta,0x02
		rcall send
		;ahora recibo los pixeles por la usart
		ldi aux,102;en realidad deberian ser 102
        
;mejorar esta parte con un ret antes y llamando
; a recibir independientemente del proceso del sensor
		
		;Le digo donde guardar los datos
		ldi	Xl,LOW(var)
		ldi Xh,HIGH(var)
recibir:
		rcall USART_Receive
	    st X+,rtn;guardo el pixel en la variable var
	    dec aux
	    brne recibir
		;Vuelvo X al principio
		ldi	Xl,LOW(var)
		ldi Xh,HIGH(var)
		ret
;*****************************************************************
;*	"send"
;*	Manda comandos al sensor.
;*	Envia comandos desde 'dta' y espera el tiempo de recepcion.
;*****************************************************************

send:    rcall USART_Transmit
		 ldi arg,0xFA
		 rcall timer;espera 6 clock para q se procese la instruccion
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
