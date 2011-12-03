;*****************************************************************
;*	File: motordef.inc
;*	
;*	Definiciones para las posiciones del motor PaP
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
   
.cseg
	.org 0x0600 
;*				//numero//	//pos anteriorh//	//pos anteriorl//	//pos posteriorh// 	//pod posterior//

posicion1:	.db 0b00000101,	high(posicion8<<1),	low(posicion8<<1),	high(posicion2<<1),	low(posicion2<<1)
posicion2:	.db 0b00000100,	high(posicion1<<1),	low(posicion1<<1),	high(posicion3<<1),	low(posicion3<<1)
posicion3:	.db 0b00000110,	high(posicion2<<1),	low(posicion2<<1),	high(posicion4<<1),	low(posicion4<<1)
posicion4:	.db 0b00000010,	high(posicion3<<1),	low(posicion3<<1),	high(posicion5<<1),	low(posicion5<<1)
posicion5:	.db 0b00001010,	high(posicion4<<1),	low(posicion4<<1),	high(posicion6<<1),	low(posicion6<<1)
posicion6:	.db 0b00001000,	high(posicion5<<1),	low(posicion5<<1),	high(posicion7<<1),	low(posicion7<<1)
posicion7:	.db 0b00001001,	high(posicion6<<1),	low(posicion6<<1),	high(posicion8<<1),	low(posicion8<<1)
posicion8:	.db 0b00000001,	high(posicion7<<1),	low(posicion7<<1),	high(posicion1<<1),	low(posicion1<<1)
