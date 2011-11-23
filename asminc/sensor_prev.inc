;*****************************************************************
;*	File: sensor_prev.inc
;*	
;*	Rutinas de control de SENSOR en el SLAVE
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
	.equ sclk=6
	.equ sdin=1
	.equ sdout=0
	.equ control=PORTD

; Instrucciones CCD
	.equ RESET_CODE =0x1B ; Main Reset
	.equ STARTINT_CODE = 0x08 ; Start Integration
	.equ SAMPLEINT_CODE = 0x10 ; Stop Integration
	.equ READPIXEL_CODE = 0x02 ; Read Pixel Data
	.equ ABORTPIXEL_CODE = 0x19 ; Abort Pixel Data Read

	.equ RW_OFFSET1_CODE = 0x40 ; Write Offset 1 Register
	.equ RW_GAIN1_CODE = 0x41 ; Write Gain 1 Register
	.equ RW_OFFSET2_CODE = 0x42 ; Write Offset 2 Register
	.equ RW_GAIN2_CODE = 0x43 ; Write Gain 2 Register
	.equ RW_OFFSET3_CODE = 0x44 ; Write Offset 3 Register
	.equ RW_GAIN3_CODE = 0x45 ; Write Gain 3 Register
	.equ RW_MODE_CODE = 0x5F ; Write Mode Register

;Valores Predeerminados
	.equ DEFAULT_MODE = 0
	.equ DEFAULT_OFFSET = 0
	.equ DEFAULT_GAIN = 0


;*****************************************************************
;*****************************************************************
;*				Rutinas para trabajar con SENSOR
;*****************************************************************
;*****************************************************************

;*****************************************************************
;*	"PROCESS_INITIALIZATION"
;*	Rutina que inicializa el sensor.
;*	
;*****************************************************************

PROCESS_INITIALIZATION:

 cbi PortB,sclk
 cbi control,sdin
 sbi control,sdout
 rcall CCD_POWERUP;Inicializo el CCD
 rcall delay1s; Espera 1 segundo para que la circuiteria del CCD se estabilice

;*****************************************************************
;*	"PROCESS_CCD"
;*	Rutina que inicializa el sensor.
;*	
;*****************************************************************
PROCESS_CCD:
; Rutina que realiza todos los procesos necesarios para que el CCD toma una foto, la
; envia al uC y el uC la guarde en memoria (la sume al total).
; Ante una duda en la secuencia, consultar hoja de datos o nota de aplicacion del CCD
; No toma argumentos ni devuelve ningun resultado.
rcall CCD_STARTINT
ldi delay,0xE2
rcall timer;hago un delay d como minimo un milisegundo
rcall CCD_SAMPLEINT
rcall CCD_READPIXEL

		;ahora recibo los pixeles por la usart
		ldi tmp,102;en realidad deberian ser 102
        
;mejorar esta parte con un ret antes y llamando
; a recibir independientemente del proceso del sensor
		
		;Le digo donde guardar los datos
		ldi	Yl,LOW(var)
		ldi Yh,HIGH(var)
recibir:
		rcall CCD_RECEIVE_BYTE
	    st Y+,rtn;guardo el pixel en la variable var
	    dec tmp
	    brne recibir
		;Vuelvo Y al principio
		ldi	Yl,LOW(var)
		ldi Yh,HIGH(var)
		ret

;*****************************************************************
;*	"CCD_POWERUP"
;*	Rutina que inicializa el sensor.
;*	
;*****************************************************************
CCD_POWERUP:
; Esta rutina ejecuta los comandos necesarios y espera los clocks necesarios para
; comenzar el funcionamiento del CCD. Esto debe realizarse cada vez que se enciende el
; CCD. Ante la duda, consultar hoja de datos y/o nota de aplicacion.
; No toma argumentos ni devuelve ningun resultado
cbi PortB,sclk
cbi control,sdin
ldi arg,30
rcall CCD_NCLOCKS
sbi control,sdin
ldi arg,10
rcall CCD_NCLOCKS
rcall CCD_RESET
ldi arg,5
rcall CCD_NCLOCKS
rcall CCD_CLEAR_MODE
ret
;*****************************************************************
;*	"CCD_POWERUP"
;*	Rutina que inicializa el sensor.
;*	
;*****************************************************************
CCD_NCLOCKS:
; Esta rutina envia la cantidad de clocks indicados al CCD.
; Toma como argumento al acumulador y no devuelve ningun resultado.
sbi PortB,sclk;
cbi PortB,sclk;
dec arg
brne CCD_NCLOCKS
ret
;*****************************************************************
;*	"CCD_POWERUP"
;*	Rutina que inicializa el sensor.
;*	
;*****************************************************************
CCD_RESET:
; Envia la instruccion de reset al CCD.
; No toma argumentos ni devuelve ningun resultado.
ldi dta,RESET_CODE
rcall CCD_LOAD_INSTRUCTION
ldi arg,6 ;Espera 6 CLKs para procesar la instruccion
rcall CCD_NCLOCKS
ret
;*****************************************************************
;*	"CCD_POWERUP"
;*	Rutina que inicializa el sensor.
;*	
;*****************************************************************
CCD_LOAD_INSTRUCTION:
; Rutina que envia un byte al CCD. A pesar del nombre de la rutina, puede enviarse un
; comando o bien un byte de datos.
; Tiene como argumento al acumulador, que es el byte a ser enviado. No devuelve ningun
; resultado.
; Para pasar el comando el bit SDIN debe cambiar cuando el CLK este bajo porque el valor
; debe estar estable cuando el CLK esta en alto.
clc;limpio el carry
cbi PortB,sclk
ldi aux,9
SET_BIT: ; Primero pone el cero del C como bit de start, luego carga el
; byte de instrucción, por eso 9
			brcc mandocero
			sbi control,sdin
continuar:  sbi PortB,sclk
			cbi PortB,sclk
			ror dta
			dec aux
			brne SET_BIT
; Bit de stop
            sbi control,sdin
            sbi PortB,sclk
            cbi PortB,sclk
            ret
mandocero: cbi control,sdin
	rjmp continuar
;*****************************************************************
;*	"CCD_POWERUP"
;*	Rutina que inicializa el sensor.
;*	
;*****************************************************************
CCD_CLEAR_MODE:
; Setea el CCD en el modo predeterminado, inicializa la ganancia y el offset con sus
; valores predeterminados.
; No toma argumentos ni devuelve ningun resultado.
; Inicializa el modo
ldi dta,RW_MODE_CODE
rcall CCD_LOAD_INSTRUCTION
ldi arg,6 ;Espera 6 CLKs para procesar la instruccion
rcall CCD_NCLOCKS
ldi dta,DEFAULT_MODE
rcall CCD_LOAD_INSTRUCTION
ldi arg,6 ;Espera 6 CLKs para procesar la instruccion
rcall CCD_NCLOCKS
; Inicializa Offset 1, 2 y 3
ldi dta,RW_OFFSET1_CODE
rcall CCD_LOAD_INSTRUCTION
ldi arg,6 ;Espera 6 CLKs para procesar la instruccion
rcall CCD_NCLOCKS
ldi dta,DEFAULT_OFFSET
rcall CCD_LOAD_INSTRUCTION
ldi arg,6 ;Espera 6 CLKs para procesar la instruccion
rcall CCD_NCLOCKS
ldi dta,RW_OFFSET2_CODE
rcall CCD_LOAD_INSTRUCTION
ldi arg,6 ;Espera 6 CLKs para procesar la instruccion
rcall CCD_NCLOCKS
ldi dta,DEFAULT_OFFSET
rcall CCD_LOAD_INSTRUCTION
ldi arg,6 ;Espera 6 CLKs para procesar la instruccion
rcall CCD_NCLOCKS
ldi dta,RW_OFFSET3_CODE
rcall CCD_LOAD_INSTRUCTION
ldi arg,6 ;Espera 6 CLKs para procesar la instruccion
rcall CCD_NCLOCKS
ldi dta,DEFAULT_OFFSET
rcall CCD_LOAD_INSTRUCTION
ldi arg,6 ;Espera 6 CLKs para procesar la instruccion
rcall CCD_NCLOCKS
; Inicializa Ganancia 1, 2 y 3
ldi dta,RW_GAIN1_CODE
rcall CCD_LOAD_INSTRUCTION
ldi arg,6 ;Espera 6 CLKs para procesar la instruccion
rcall CCD_NCLOCKS
ldi dta,DEFAULT_GAIN
rcall CCD_LOAD_INSTRUCTION
ldi arg,6 ;Espera 6 CLKs para procesar la instruccion
rcall CCD_NCLOCKS
ldi dta,RW_GAIN2_CODE
rcall CCD_LOAD_INSTRUCTION
ldi arg,6 ;Espera 6 CLKs para procesar la instruccion
rcall CCD_NCLOCKS
ldi dta,DEFAULT_GAIN
rcall CCD_LOAD_INSTRUCTION
ldi arg,6 ;Espera 6 CLKs para procesar la instruccion
rcall CCD_NCLOCKS
ldi dta,RW_GAIN3_CODE
rcall CCD_LOAD_INSTRUCTION
ldi arg,6 ;Espera 6 CLKs para procesar la instruccion
rcall CCD_NCLOCKS
ldi dta,DEFAULT_GAIN
rcall CCD_LOAD_INSTRUCTION
ldi arg,6 ;Espera 6 CLKs para procesar la instruccion
rcall CCD_NCLOCKS
ret
;*****************************************************************
;*	"CCD_POWERUP"
;*	Rutina que inicializa el sensor.
;*	
;*****************************************************************
CCD_STARTINT:
; Envia la instruccion para comenzar la integracion de luz al CCD.
; No toma argumentos ni devuelve ningun resultado.

ldi dta,STARTINT_CODE
rcall CCD_LOAD_INSTRUCTION
ldi arg,6 ;Espera 6 CLKs para procesar la instruccion
rcall CCD_NCLOCKS
ldi arg,22 ;Espera otros 22 CLKs como indica la nota de aplicacion
rcall CCD_NCLOCKS
ret
;*****************************************************************
;*	"CCD_POWERUP"
;*	Rutina que inicializa el sensor.
;*	
;*****************************************************************
CCD_SAMPLEINT:
; Envia la instruccion para finalizar la integracion de luz y digitalización de la señal
; al CCD.
; No toma argumentos ni devuelve ningun resultado.
ldi dta,SAMPLEINT_CODE
rcall CCD_LOAD_INSTRUCTION
ldi arg,6 ;Espera 6 CLKs para procesar la instruccion
rcall CCD_NCLOCKS
ldi arg,5 ;Espera otros 5 CLKs como indica la nota de aplicacion
rcall CCD_NCLOCKS
ret
;*****************************************************************
;*	"CCD_POWERUP"
;*	Rutina que inicializa el sensor.
;*	
;*****************************************************************
CCD_READPIXEL:
; Envia la instruccion para comenzar la comunicacion serie por SOUT al CCD.
; No toma argumentos ni devuelve ningun resultado.
ldi dta,READPIXEL_CODE
rcall CCD_LOAD_INSTRUCTION
ret
;*****************************************************************
;*	"CCD_POWERUP"
;*	Rutina que inicializa el sensor.
;*	
;*****************************************************************
CCD_RECEIVE_BYTE:
; Rutina que recibe el Byte que envia el CCD indicando la cantidad de luz "leida" por el pixel.
; No toma argumentos, pero devuelve el Byte recibido por el rtn

	sbis control,sdout; Si SDOUT esta en bajo, es porque llego el bit de Start
	rjmp LOAD_BYTE
CHECK_START_BIT: ; Si no, espera a que llegue el bit de start
	sbi PortB,sclk
	cbi PortB,sclk
	sbic control,sdout
	rjmp CHECK_START_BIT
LOAD_BYTE: ; Luego de recibir el bit de start, va a recibir los 8 bits
; de datos
	ldi aux,8
GET_BIT:
	sbi PortB,sclk
	cbi PortB,sclk
	sbis control,sdout;esquivo la prox si sdout es 1
	rjmp bit_es_cero
	sec;pongo el carry en 1
roto:
	ror rtn;voy rotando return
	dec aux
	brne GET_BIT
	sbi PortB,sclk
	cbi PortB,sclk;Deja pasar el bit d STOP
	ret
bit_es_cero:
	clc;pongo el carry en cero
rjmp roto