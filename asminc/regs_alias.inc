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

	.def    max     =    r0
	.def	low_suma=	 r2
	.def	high_suma=	 r3
	.def	rBin1L	=	 r6
	.def	rBin1H	=	 r7
	.def	rBin2L	=	 r4
	.def	rBin2H	=	 r5
	.def	zero	=	 r8		
	.def	id 		=	 r9 		;Id posicion menu
	.def	wrdl	=	 r10
	.def	wrdh 	=	 r11
	.def	lambdaL	=	 r12
	.def	lambdaH	=	 r13
	.def	tmp		=	 r16		;* Registro para uso general
	.def	arg		=	 r17		;* Argumentos para pasar a funciones
	.def	rtn		=	 r18		;valor de retorno de subrutinas
	.def    delay	=	 r19		;valor de bayte recivido por SPI
	.def	fla		=	 r20		;Registro para flags:b(0-1):Opcion global.b2:switchUPon
								;b3:switchDWon ;b4:switchRTon; b5:on/off; b6-7 tipo de menu
	.def	key		=	r21
	.def	dta		=	r22		
	.def 	dtal	=	r23
	.def 	aux 	=	r24

	.def 	con		=	r25		; hay que reemplazar por r[0-15]
