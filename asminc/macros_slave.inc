	;* 1byte calibrado? 		(primer nibble en 0xF indica que hice el primer valor)
	;*							(segundo nibble en 0xF, es decir 0xFF de este byte indica)
	;*							(que ya calibre el aparato, doy la opcion de volver a)
	;*							(calibrar si se desea...)
	;* 2byte lambda1			(primer valor de lambda)
	;* 2byte calibra1 			(su posicion de motor)
	;* 2byte lambda2 			(segundo valor de lambda)
	;* 2byte calibra2	(su posicion de motor)
	;* 2byte deltaPasoXlambda 	(cuantos pasos para aumentar lambda en 1??)
	;* 2byte posMinLambda 		(400||200 segun como lo quieran, minimo de sensor)
	;*							(o minimo espectro visible???)

	;* .equ calibra1x = 3
	;* MoveCalibraTo calibra1x

	;Esta macro espera (una vez definida la memoria EEPROM)
	;Que cumpla el mismo objetivo que MoveZTo

	;Requiere que se definan las constantes equivalentes
	;A posiciones en bytes de memoria eeprom!!


	.MACRO MoveCalibraTo
		adiw 	Yh:Yl,@0
	.ENDMACRO
