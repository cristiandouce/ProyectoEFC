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
	.org 0x0600    									

		;direccion:			prev(2)//next(2)//return(2)//firstlinehandler//secondlinehandler//fstline//scdline
		Lectura:	.db		high(check_keys),low(check_keys),high(lCalibra),low(lCalibra),high(ComunicateL),low(ComunicateL),high(flstd),low(flstd),high(slstd),low(slstd),high(sLectura<<1),low(sLectura<<1),high(sCalibra<<1),low(sCalibra<<1)
		sLectura:	.db 	"Lectura",fst
		Resultados: .db 	high(Lambda_Up),low(Lambda_Up),high(Lambda_Down),low(Lambda_Down),high(lLectura),low(lLectura),high(dispintensidad),low(dispintensidad),high(slLambda),low(slLambda),0,0,0,0
		Pixeles: 	.db 	high(Pixel_Up),low(Pixel_Up),high(Pixel_Down),low(Pixel_Down),high(lProbarP),low(lProbarP),high(dispintensidad),low(dispintensidad),high(slPixel),low(slPixel),0,0,0,0
		Calibra:	.db		high(lLectura),low(lLectura),high(lProbarM),low(lProbarM),high(toLambda),low(toLambda),high(flstd),low(flstd),high(slstd),low(slstd),high(sCalibra<<1),low(sCalibra<<1),high(sProbarM<<1),low(sProbarM<<1)
		sCalibra: 	.db 	"Calibracion",fst
		MotorM: 	.db		high(mot_up),low(mot_up),high(mot_down),low(mot_down),high(lProbarM),low(lProbarM),high(flstd),low(flstd),high(slstd),low(slstd),high(sProbando<<1),low(sProbando<<1),high(sProbando<<1),low(sProbando<<1)
		sProbando: 	.db 	"Probando motor",fst
		ProbarM:	.db		high(lCalibra),low(lCalibra),high(lProbar),low(lProbar),high(comunicateM),low(comunicateM),high(flstd),low(flstd),high(slstd),low(slstd),high(sProbarM<<1),low(sProbarM<<1),high(sProbar<<1),low(sProbar<<1)
		sProbarM: 	.db 	"Probar motor",fst
		Probar: 	.db		high(lProbarM),low(lProbarM),high(lVerCal),low(lVerCal),high(ComunicateQ),low(ComunicateQ),high(flstd),low(flstd),high(slstd),low(slstd),high(sProbar<<1),low(sProbar<<1),high(sVerCal<<1),low(sVerCal<<1)
		sProbar: 	.db 	"Num.Pasos Motor",fst
		VerCal: 	.db		high(lProbar),low(lProbar),high(lResCal),low(lResCal),high(ComunicateV),low(ComunicateV),high(flstd),low(flstd),high(slstd),low(slstd),high(sVerCal<<1),low(sVerCal<<1),high(sResCal<<1),low(sResCal<<1)
		sVerCal: 	.db 	"Ver calibracion",fst
		ResCal: 	.db		high(lVerCal),low(lVerCal),high(lFakeCal),low(lFakeCal),high(ComunicateR),low(ComunicateR),high(flstd),low(flstd),high(slstd),low(slstd),high(sResCal<<1),low(sResCal<<1),high(sFakeCal<<1),low(sFakeCal<<1)
		sResCal: 	.db 	"Rst.Calibracion",fst
		FakeCal: 	.db		high(lResCal),low(lResCal),high(lProbarP),low(lProbarP),high(ComunicateF),low(ComunicateF),high(flstd),low(flstd),high(slstd),low(slstd),high(sFakeCal<<1),low(sFakeCal<<1),high(sProbarP<<1),low(sProbarP<<1)
		sFakeCal: 	.db 	"Fake Calib.",fst
		ProbarP: 	.db		high(lFakeCal),low(lFakeCal),high(lLamp),low(lLamp),high(ComunicateF),low(ComunicateF),high(flstd),low(flstd),high(slLamp),low(slLamp),high(sProbarP<<1),low(sProbarP<<1),0,0
		sProbarP:	.db		"Medir sensor",fst
		Lampara:	.db		high(lVerCal),low(lVerCal),high(check_keys),low(check_keys),high(comunicateP),low(comunicateP),high(flend),low(flend),high(slLamp2),low(slLamp2),high(sProbarP<<1),low(sProbarP<<1),0,0
		sLampOn:	.db		"Lamp.*On*- Off ",fst
		sLampOff:	.db		"Lamp. On -*Off*",fst
		Lambda:		.db		high(Lambda_Up),low(Lambda_Up),high(Lambda_Down),low(Lambda_Down),high(Lambda_Ret),low(Lambda_Ret),high(flstd),low(flstd),high(slLambda),low(slLambda),high(sLambda<<1),low(sLambda<<1),0,0
		sLambda:	.db 	"Ingrese Lambda: ",fst
		sBusy:		.db		"Leyendo...",fst
		sBusyC:		.db		"Procesando...",fst
		sExp:		.db		"Exportar a PC? ",fst
		sDone:		.db		"Listo",fst
		sError:		.db		"Error!!",fst
		sLo: 		.db 	"L.O.:",fst
		sInt: 		.db		"Int.:",fst
		sEnviandoPC:.db		"Enviando a la PC",fst
 		sEstado:	.db		"Estado:",fst
 		sInimot: 	.db 	"Iniciando motor",fst