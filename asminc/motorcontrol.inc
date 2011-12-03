;*****************************************************************
;*	File: motorcontrol.inc
;*	
;*	Rutinas para controlar el motor PaP
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
;*				Rutinas para trabajar con Motor PaP
;*****************************************************************
;*****************************************************************

;*****************************************************************
;*	"contarpasos"
;*	Cuenta los pasos del motor.
;*	
;*****************************************************************

contarpasos: 	clr   	Yl
				clr		Yh
iniciarmotor1:	rcall 	stepforth
				rcall 	delayY
				in 		tmp, PINB
				ldi 	aux, 0b00000010
				and 	tmp, aux
				cpi 	tmp, 0
				brne 	iniciarmotor1
testpasos:
				; Cada cuantos pasos cuento
				ldi		r23,1
				; Empiezo inicializacion y
				; cuento los pasos en el camino
iniciarmotor2:	rcall 	stepback
				rcall 	delayY
				in 		tmp, PINB
				ldi 	aux, 0b01000000
				and 	tmp, aux
				cpi 	tmp, 0
				breq 	terminarmotor
				dec 	r23
				brne 	iniciarmotor2
				adiw 	Yh:Yl, 1
				rjmp testpasos
				rjmp 	iniciarmotor2
terminarmotor:	ldi 	xh, high(cant_pasos)
				ldi 	xl, low(cant_pasos)
				st 		X+, Yl
				st 		X+, Yh
				ret


;*****************************************************************
;*	"stepforth"
;*	Avanza el motor un paso.
;*	
;*****************************************************************

stepforth:
		
		movw 	Zh:Zl, wrdh:wrdl
		lpm 	tmp, Z+
		in 		aux, portc
		cbr 	aux, 0b00001111
		or 		tmp, aux
		out 	portc, tmp
		adiw	ZH:ZL, 2
		lpm		wrdh, Z+
		lpm		wrdl, Z+
		ret


;*****************************************************************
;*	"stepback"
;*	Retrocede el motor un paso.
;*	
;*****************************************************************

stepback:
		movw 	Zh:Zl, wrdh:wrdl
		lpm 	tmp, Z+
		in 		aux, PortC
		cbr 	aux, 0b00001111
		or 		tmp, aux
		out 	PortC, tmp
		lpm		wrdh, Z+
		lpm		wrdl, Z+
		ret