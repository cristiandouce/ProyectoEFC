;*****************************************************************
;*	File: eeprom.inc
;*	
;*	Rutinas comunes en ambos micros
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
;*****************************************************************
;*				Rutinas para trabajar con EEPROM
;*****************************************************************
;*****************************************************************

;*****************************************************************
;*	"EEPROM_write"
;*	Escribe en memoria EEPROM.
;*	Recive en Yh:Yl la direccion sobre la cual escribir
;* 	y guarda el valor transportado dentro de 'arg'
;*****************************************************************

EEPROM_write:
		; Wait for completion of previous write
		sbic EECR,EEPE 
		rjmp EEPROM_write ; Set up address (Yh:Yl) in address register 
		out EEARH, Yh 
		out EEARL, Yl ; Write data (arg) to Data Register 
		out EEDR,arg ; Write logical one to EEMPE 
		sbi EECR,EEMPE ; Start eeprom write by setting EEPE 
		sbi EECR,EEPE 
		ret

;*****************************************************************
;*	"EEPROM_read"
;*	Escribe en memoria EEPROM.
;*	Recive en Yh:Yl la direccion de la cual leer
;* 	y lee el valor en memoria devolviendolo en 'rtn'
;*****************************************************************

EEPROM_read:
		; Wait for completion of previous write
		sbic EECR,EEPE 
		rjmp EEPROM_read ; Set up address (Yh:Yl) in address register 
		out EEARH, Yh 
		out EEARL, Yl ; Start eeprom read by writing EERE 
		sbi EECR,EERE ; Read data from Data Register 
		in rtn,EEDR 
		ret