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

	.def	tmp		=	r16
	.def	arg		=	r17		;*	argument for calling subroutines
	.def	rtn		=	r18		;*	rtn value from subroutines
	.def    dta		=	r19		;*	el q le da el master
	.def	tmt		=	r20
	.def	rcv		=	r21
	.def	usd		=	r22
	.def 	delay	=	r23
	.def 	pixel 	=	r24

