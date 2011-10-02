;*****************************************************************
;*	File: spi_slave.inc
;*	
;*	Rutinas de control de SPI en modo SLAVE
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
;-Estados
	.equ	op1			=	0x00		;opcion1: lectura en menu principal; 'Si' en exportar y calibracion 2
	.equ	op2			=	0x01		;opcion2: calibracion en menu principal: 'No' en exportar y calibracion 2	
	.equ	op3			=	0x02		;opcion3: Lampara on en menu principal: 'Volver' en calibracion 2
	.equ	op4			=	0x03		;opcion4	calibracion1
	.equ	menu1		=	0b00000000	;Menu inicial
	.equ	menu2		=	0b01000000	;Menu exportar
	.equ	menu3		=	0b10000000	;Menu Calibracion 
	.equ	menu4		=	0b11000000	;Menu Medición
	.equ	maskOP		=	0b00000011	;para leer flags
	.equ	maskRESET	=	0b00011100
	.equ	flagUP		=	2
	.equ	flagDW		=	3
	.equ	flagRT		=	4
	.equ	flagLmp		=	5
	.equ	maskUP		=	0b00000100
	.equ	maskDW		=	0b00001000
	.equ	maskRT		=	0b00010000
	.equ	maskLmp		=	0b00100000
	.equ	maskMN		=	0b11000000
;-Pines puerto C
	.equ	lamp		= 	0
	.equ	switchUP	= 	2
	.equ	switchDW	= 	3
	.equ	switchRT	= 	4
;*****************************************************************
;*	VARIABLES
;*****************************************************************
	.dseg
		string:	.byte	17
;*****************************************************************
;*	CADENAS DE CARACTERES
;*****************************************************************
	.cseg
		.org 0x0400    									;Guarda las tablas en 0x0800 de Memoria de Programa (?)
			strLec:		.db	"Lectura",fst				;opcion lectura
			strCal:		.db	"Calibracion",fst			;opcion calibracion
			strLmpOn:	.db	"Lamp.*On*-Off",fst			;Lampara on
			strLmpOff:	.db	"Lamp.On-*Off*",fst			;Lampara on
			strLmd:		.db	"Ingrese Lambda ",fst
			strCL:		.db	"Confirma lambda?",fst,fst
			strSi:		.db	"Si",fst,fst
			strNo:		.db	"No",fst,fst
			strBck:		.db	"Volver",fst,fst
			strBsy:		.db	"Leyendo",fst
			strExp:		.db	"Exportar a PC? ",fst
			strDone:	.db	"Listo",fst
			strError:	.db	"Error!!",fst
			