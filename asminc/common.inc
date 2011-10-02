;*****************************************************************
;*	File: common.inc
;*	
;*	Rutinas comunes en ambos micros
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
;*				Rutinas Comunes a los dos micros
;*****************************************************************
;*****************************************************************

;*****************************************************************
;*	"delay200ms"
;*	Rutina de DELAY
;*	Duracion de 197ms
;*****************************************************************
delay200ms:
			clr	r2
delay200ms_outer:
			clr	r3
delay200ms_inner:
			dec r3
			brne delay200ms_inner 		; sale del ciclo si r3 es igual a cero
			dec	r2						;cuando ya hizo 255us decremento r2
			brne delay200ms_outer 		;sale dl ciclo si r2 es igual a cero
			ret


;*****************************************************************
;*	"delay1s"
;*	Rutina de DELAY
;*	Duracion de 1s aprox
;*****************************************************************
delay1s:
		rcall delay200ms
		rcall delay200ms
		rcall delay200ms
		rcall delay200ms
		rcall delay200ms
		ret
