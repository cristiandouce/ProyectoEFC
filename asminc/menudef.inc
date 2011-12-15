;*****************************************************************
;*	File: menudef.inc
;*	
;*	Definiciones para el manejo del Menu por LCD 16x2
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
	.equ	maskOP		=	0b00000011	;para leer flags
	.equ	maskRESET	=	0b00011100
	.equ	flagUP		=	2
	.equ	flagDW		=	3
	.equ	flagFW		=	4
	.equ	flagLmp		=	5
	.equ	maskUP		=	0b00000100
	.equ	maskDW		=	0b00001000
	.equ	maskRT		=	0b00010000
	.equ	maskLmp		=	0b00100000
;-Pines puerto C
	.equ	lamp		= 	0
	.equ	switchUP	= 	2
	.equ	switchDW	= 	3
	.equ	switchRT	= 	4
;-Cadenas de caracteres
	.equ	fst			=	0 			;fin de string en NULL character


	.equ	prev		=	0
	.equ	next		=	2
	.equ	fwd			=	4
	.equ 	firstlinehandler = 6
	.equ 	secondlinehandler = 8
	.equ 	fstline = 10
	.equ 	scdline = 12

;*****************************************************************
;*	VARIABLES
;*****************************************************************
	.dseg
		buffer: 		.byte 	5
		datos: 			.byte 	800
		valor_lambda:	.byte 	2

;*****************************************************************
;*	CADENAS DE CARACTERES
;*****************************************************************
   
.cseg
	.org 0x0400    									

		;direccion:			prev(2)//next(2)//return(2)//firstlinehandler//secondlinehandler//fstline//scdline
		Lectura:	.db		high(check_keys),low(check_keys),high(lCalibra),low(lCalibra),high(ComunicateL),low(ComunicateL),high(flstd),low(flstd),high(slstd),low(slstd),high(sLectura<<1),low(sLectura<<1),high(sCalibra<<1),low(sCalibra<<1)
		sLectura:	.db 	"Lectura",fst
		Resultados: .db 	high(Lambda_Up),low(Lambda_Up),high(Lambda_Down),low(Lambda_Down),high(lLectura),low(lLectura),high(dispintensidad),low(dispintensidad),high(slLambda),low(slLambda),0,0,0,0
		Calibra:	.db		high(lLectura),low(lLectura),high(lProbarM),low(lProbarM),high(toLambda),low(toLambda),high(flstd),low(flstd),high(slstd),low(slstd),high(sCalibra<<1),low(sCalibra<<1),high(sProbarM<<1),low(sProbarM<<1)
		MotorM: 	.db		high(mot_up),low(mot_up),high(mot_down),low(mot_down),high(lProbarM),low(lProbarM),high(flstd),low(flstd),high(slstd),low(slstd),high(sProbando<<1),low(sProbando<<1),high(sProbando<<1),low(sProbando<<1)
		sProbando: 	.db 	"Probando motor",fst
		ProbarM:	.db		high(lCalibra),low(lCalibra),high(lProbar),low(lProbar),high(comunicateM),low(comunicateM),high(flstd),low(flstd),high(slstd),low(slstd),high(sProbarM<<1),low(sProbarM<<1),high(sProbar<<1),low(sProbar<<1)
		sProbarM: 	.db 	"Probar motor",fst
		Probar: 	.db		high(lProbarM),low(lProbarM),high(lLamp),low(lLamp),high(ComunicateQ),low(ComunicateQ),high(flstd),low(flstd),high(slLamp),low(slLamp),high(sProbar<<1),low(sProbar<<1),0,0
		sProbar: 	.db 	"Cantidad de pasos",fst
		sCalibra: 	.db 	"Calibracion",fst
		Lampara:	.db		high(lProbar),low(lProbar),high(check_keys),low(check_keys),high(switchLamp),low(switchLamp),high(flend),low(flend),high(slLamp2),low(slLamp2),high(sProbar<<1),low(sProbar<<1),0,0
		sLampOn:	.db		"Lamp.*On*- Off ",fst
		sLampOff:	.db		"Lamp. On -*Off*",fst
		Lambda:		.db		high(Lambda_Up),low(Lambda_Up),high(Lambda_Down),low(Lambda_Down),high(Lambda_Ret),low(Lambda_Ret),high(flstd),low(flstd),high(slLambda),low(slLambda),high(sLambda<<1),low(sLambda<<1),0,0
		sLambda:	.db 	"Ingrese Lambda: ",fst
		BusyC:		.db		high(idle),low(idle),high(idle),low(idle),high(idle),low(idle),high(flstd),low(flstd),high(comunicateC),low(comunicateC),high(sBusyC<<1),low(sBusyC<<1),0,0
		sBusy:		.db		"Leyendo",fst
		sBusyC:		.db		"Procesando",fst
		sExp:		.db		"Exportar a PC? ",fst
		sDone:		.db		"Listo",fst
		sError:		.db		"Error!!",fst
		sLo: 		.db 	"L.O.:",fst
		sInt: 		.db		"Int.:",fst
		sEnviandoPC:.db		"Enviando a la PC",fst
 