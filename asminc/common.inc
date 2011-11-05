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
;*****************************************************************
;*	"div8"
;*	Rutina de Division 16bitsx8bits
;*	16bitsx8bits
;*****************************************************************

div8:
	clr rd1u ; clear interim register
	clr reh ; clear result (the result registers
	clr rel ; are also used to count to 16 for the
	inc rel ; division steps, is set to 1 at start)
;
; Here the division loop starts
;
div8a:
	clc ; clear carry-bit
	rol rd1l ; rotate the next-upper bit of the number
	rol rd1h ; to the interim register (multiply by 2)
	rol rd1u
	brcs div8b ; a one has rolled left, so subtract
	cp rd1u,rd2 ; Division result 1 or 0?
	brcs div8c ; jump over subtraction, if smaller
div8b:
	sub rd1u,rd2; subtract number to divide with
	sec ; set carry-bit, result is a 1
	rjmp div8d ; jump to shift of the result bit
div8c:
	clc ; clear carry-bit, resulting bit is a 0
div8d:
	rol rel ; rotate carry-bit into result registers
	rol reh
	brcc div8a ; as long as zero rotate out of the result
	 ; registers: go on with the division loop
; End of the division reached
stop:
	rjmp stop ; endless loop