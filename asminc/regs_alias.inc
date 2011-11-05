;*****************************************************************
;*	File: lcd_driver.inc
;*	
;*	Driver para LCD con 4 (cuatro) bit de datos
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
;*	ALIAS DE REGISTROS
;*****************************************************************
	
	.def 	rd1l 	=	 r0 		; numero de 16 bit a dividir -low
	.def 	rd1h 	=	 r1 		; numero de 16 bit a dividir -low
	.def 	rd1u 	=	 r2 		; registro extra para division
	.def 	rd2 	=	 r3 		; numero de 8-bit para dividir
	.def 	rel 	=	 r4 		; resultado division
	.def 	reh 	=	 r5 		; resultado division
	.def	zero	=	 r8		
	.def	id 		=	 r9 		;Id posicion menu
	.def	wrdl	=	 r10
	.def	wrdh 	=	 r11
	.def	lambdaH	=	 r12
	.def	lambdaL	=	 r13
	.def	tmp		=	 r16		;* Registro para uso general
	.def	arg		=	 r17		;* Argumentos para pasar a funciones
	.def	rtn		=	 r18		;valor de retorno de subrutinas
	.def    dta		=	 r19		;valor de bayte recivido por SPI
	.def	fla		=	 r20		;Registro para flags:b(0-1):Opcion global.b2:switchUPon
								;b3:switchDWon ;b4:switchRTon; b5:on/off; b6-7 tipo de menu
	.def	key		=	r21
	.def	lmd		=	r22		
	.def 	delay	=	r23
	.def 	aux 	=	r24

	.def 	con		=	r25		; hay que reemplazar por r[0-15]
