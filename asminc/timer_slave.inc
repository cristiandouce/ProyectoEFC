;*****************************************************************
;*	File: timer_slave.inc
;*	
;*	Rutinas de control del TIMER en el SLAVE
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
;*	"timer"
;*	Rutina setea un timer.
;*	Setea tiempo segun parametro en arg
;*****************************************************************

timer:ldi tmp,0x01
	  out TCCR0B,tmp;sin prescaler y prendo el timer
      out TCNT0,arg;digo cuanto voy a contar
	  rcall contador
	  ldi tmp,0x00
	  out TCCR0B,tmp;apago el timer
	  cbi TIFR0,0;pongo d nuevo el flag en cero
	  ret

;*****************************************************************
;*	"contador"
;*	Rutina contadora para timer.
;*	Espera a que se termine d contar y vuelve a la subrutina timer
;*****************************************************************

contador: sbis TIFR0,0
          rjmp contador
		  ret
