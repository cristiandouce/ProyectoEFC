
;****************************************************************
;*****************************************************************
;*       Definición de variables utilizadas
;*****************************************************************
;*****************************************************************

.dseg

;luego de inicializar el micro, cuenta los pasos del motor

pos_max: .byte 2

lambda1:     .byte   2
;deberia estar en el main y se va llenando con las llamadas 
;consecutivas a calibrar

lambda2:     .byte  2

delta_lambda: .byte 2

mediciones:  .byte 500
;preguntar cuantos valores guardar,esto seria cada 16 pasos




calibra1: .byte 2

calibra2: .byte 2
 
delta_paso: .byte 2

.cseg

;****************************************************************
;*****************************************************************
;*       Definición de registros utilizadas
;*****************************************************************
;*****************************************************************


 .def max=r0; no esta definido asi q lo puedo usar :D
   
;****************************************************************
;*****************************************************************
;*				Rutinas para trabajar con ESPECTROFOTOCOLIMETRO
;*****************************************************************
;*****************************************************************

;*****************************************************************
;*	"calibracion"
;*	Rutina que guarda la posicion del motor para la cual la intensidad es maxima
;*	Devuelve en la variable pos_max la posicion del motor para la cual la intensidad es maxima
;   Utiliza los registros rtn,max,X,Y,Z

;*****************************************************************
calibracion:
            ldi Xl,low(cant_pasos)
            ldi Xh,high(cant_pasos)
            ld Zh,X+;asi estoy usando el puntero Z
            ld Zl,X
            clr Yh;parte alta de pos_max, éste es el puntero Y
            clr Yl;parte de baja de pos_max
            clr max
                        
cal:        rcall sensor;sensor me devuelve el valor procesado en rtn
            rcall procesar_lectura

            cp max,rtn
            brcs actualizar_maximo
cont:       adiw r29:r28,1;incremento la posicion del vector
            rcall stepforth;avanzo un paso con el motor
            rcall delayY
            sbiw r31:r30,1;disminuyo el contador
            brne cal
             ret

actualizar_maximo:
            mov max,rtn
            ldi Xl,low(pos_max)
            ldi Xh,high(pos_max)
            st X+,r29
            st X,r28
            rjmp cont

;*****************************************************************
;*	"lectura"
;*	Rutina que realiza el espectro de una fuente de luz
;   Guarda los valores en la variable mediciones
;*	Toma los valores cada dieciseis pasos del motor
;*****************************************************************
lectura:
 	   	      ldi Xl,low(cant_pasos)
            ldi Xh,high(cant_pasos)
            ld r31,X+;guardo en z la cantidad de pasos
            ld r30,X
        
            ldi Xl,low(mediciones)
            ldi Xh,high(mediciones)
sigo_midiendo: 
            rcall sensor
            st X+,rtn
       	    ldi tmp,16	 
diez_pasos:
            rcall stepforth
            dec tmp
            brne diez_pasos
       	    sbiw r31:r30,16
       	    brne sigo_midiendo 
      	ret

 ;*****************************************************************
;*	"conversion"
;*	Rutina que con los dos valores de lamba establece una equivalencia entre las posiciones del vector de mediciones y sus longitudes de onda
;*   Devuelve en rtn cuantas longitudes de onda equivalen a 16 pasos del motor
;  
;*	
;*****************************************************************
conversion:
           ldi Xl,low(lambda1)
           ldi Xh,high(lambda1) 
           ld r1,X+
           ld r0,X;guardo en r1:r0 el valor de lambda 1
           ldi Xl,low(lambda2)
           ldi Xh,high(lambda2)
           ld r3,X+
           ld r2,X;guardo en r3:r2 el valor de lambda 2

           rcall resta
           ;esta funcion resta los dos y me devuelve el valor absoluto en r1:r0
           
           ldi Xl,low(delta_lambda)
           ldi Xh,high(delta_lambda)
           ldi tmp,4
multiplicar:
           lsl r0;Multiply r1:r0 by two
           rol r1
           dec tmp
           brne multiplicar 

           st X+,r1;guardo el valor multiplicado x 16
           st X,r0

           ldi Xl,low(calibra1)
           ldi Xh,high(calibra1) 
           ld r1,X+
           ld r0,X;guardo en r1:r0 el valor de calibra 1
           ldi Xl,low(calibra2)
           ldi Xh,high(calibra2)
           ld r3,X+
           ld r2,X;guardo en r3:r2 el valor de calibra 2
           
           rcall resta  
           ;me devuelve el valor absoluto en r1:r0
           ldi Xl,low(delta_paso)
           ldi Xh,high(delta_paso)
           st X+,r1
           st X,r0

            ;ahora hago la regla de 3 simple, tengo q dividir delta_lambda/delta_paso
            
            ldi Xl,low(delta_lambda)
            ldi Xh,high(delta_lambda)
            ld r1,X+
            ld r0,X
            ldi Xl,low(delta_paso)
            ldi Xh,high(delta_paso)
            ld r3,X+
            ld r2,X   
            rcall div16_16
            ;el resultado queda en  r3:r2
            ;como me va a quedar un numero de un byte 
            ;descarto el byte alto
            mov rtn,r2
            ret

 ;***************************************************************************************************************************************  
 ;*****************************************************************
;* "resta"
;   *Rutina que resta r r1:r0 con r3:r2, y devuelve el valor absoluto de la resta 
;   *Entrega  el valor absoluto en r1:r0
;  
;*  
;*****************************************************************         

 resta:
       
       cp r1,r3
       ;comparo las partes altas para ver quien es mayor    
       breq son_iguales
       brcs segundo_es_mayor
 primero_es_mayor:
        ;Subtract r3:r2  from r1:r0

        sub r0,r2; Subtract low byte
        sbc r1,r3; Subtract with carry high byte
 volver:
         ret


 segundo_es_mayor: 
        ;Subtract r1:ro from r3:r2
        sub r2,r0;Subtract low byte
        sbc r3,r1;Subtract with carry high byte
        mov r0,r2
        mov r1,r3
        rjmp volver
 
 son_iguales:
         cp r0,r2
         brcs r2esmayor
 r0esmayor:
         sub r0,r2
         clr r1
         rjmp volver

 r2esmayor:
          sub r2,r0
          clr r1
          mov r0,r2
          rjmp volver                                       
;*******************************************************************   
;*****************************************************************
;*  "div16_16"
;*  Rutina que divide dos bytes por dos bytes
;   Utiliza los registros de r0 a r7
;    Tambien utiliza tmp y aux
;*  El divisor tiene que estar en r3:r2
;*  El dividendo tiene que estar en r1:r0
;*  Devuelve el resultado en r3:r2
;*****************************************************************


div16_16:
  clc     ;Clear carry initially
  clr r4 ;Clear R4 working variable initially
  clr r5 ;CLear R5 working variable initially
  clr aux ;Clear aux since aux will count the number of left-shifted bits
div1:
  inc aux      ;Increment counter for each left shift
  mov tmp,r2   ;Move the current divisor low byte into the accumulator
  rol tmp      ;Shift low-byte left, rotate through carry to apply highest bit to high-byte
  mov r2,tmp   ;Save the updated divisor low-byte
  mov tmp,r3   ;Move the current divisor high byte into the accumulator
  rol tmp     ;Shift high-byte left high, rotating in carry from low-byte
  mov r3,tmp   ;Save the updated divisor high-byte
  brcc div1   ;Repeat until carry flag is set from high-byte
div2:        ;Shift right the divisor
  mov tmp,r3   ;Move high-byte of divisor into accumulator
  ror tmp      ;Rotate high-byte of divisor right and into carry
  mov r3,tmp   ;Save updated value of high-byte of divisor
  mov tmp,r2   ;Move low-byte of divisor into accumulator
  ror tmp      ;Rotate low-byte of divisor right, with carry from high-byte
  mov r2,tmp   ;Save updated value of low-byte of divisor
  clc      ;Clear carry, we don't need it anymore
  mov r7,r1 ;Make a safe copy of the dividend high-byte
  mov r6,r0 ;Make a safe copy of the dividend low-byte
  mov tmp,r0   ;Move low-byte of dividend into accumulator
  sbc tmp,r2  ;Dividend - shifted divisor = result bit (no ftmpctor, only 0 or 1)
  mov r0,tmp   ;Save updated dividend 
  mov tmp,r1   ;Move high-byte of dividend into accumulator
  sbc tmp,r3  ;Subtract high-byte of divisor (all together 16-bit substraction)
  mov r1,tmp   ;Save updated high-byte back in high-byte of divisor
  brcc div3   ;If carry flag is NOT set, result is 1
  mov r1,r7 ;Otherwise result is 0, save copy of divisor to undo subtraction
  mov r0,r6
div3:
  brcc setear
  clc      ;Invert carry, so it can be directly copied into result
continuar:  
  mov tmp,r4
  rol tmp      ;Shift carry flag into temporary result
  mov r4,tmp   
  mov tmp,r5
  rol tmp
  mov r5,tmp
  dec aux 
  brne div2;Now count backwards and repeat until "B" is zero
  mov r3,r5  ;Move result to R3/R2
  mov r2,r4  ;Move result to R3/R2
  ret

setear: 
       sec
       rjmp continuar
;/******************************************************************/       