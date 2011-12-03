;*****************************************************************
;*	File: procesar_lectura.inc
;*	
;*	Rutina para procesar lectura del sensor
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

;*!!
;*  VER SI NO CONVIENE INCLUIR ESTA RUTINA
;*  EN ALGUN OTRO INCLUDE YA EXISTENTE... (ELIMINANDO ASI SALTOS
;*  GRANDES)
;*!!
 
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