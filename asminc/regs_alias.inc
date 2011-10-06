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

	.def	tmp		=	r16		;* Registro para uso general
	.def	arg		=	r17		;* Argumentos para pasar a funciones
	.def	rtn		=	r18		;valor de retorno de subrutinas
	.def    dta		=	r19		;valor de bayte recivido por SPI
	.def	fla		=	r20		;Registro para flags:b(0-1):Opcion global.b2:switchUPon
								;b3:switchDWon ;b4:switchRTon; b5:on/off; b6-7 tipo de menu
	.def	key		=	r21
	.def	lmd		=	r22		
	.def 	delay	=	r23
	.def 	aux 	=	r24

	.def 	con		=	r25		; hay que reemplazar por r[0-15]
