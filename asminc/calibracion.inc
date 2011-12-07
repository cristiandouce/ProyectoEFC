;***************************************************************
;*****************************************************************
;*				Rutinas para trabajar con ESPECTROFOTOCOLIMETRO
;*****************************************************************
;*****************************************************************

;*****************************************************************
;*	"calibracion"
;*	Rutina que guarda la posicion del motor para la cual la intensidad es maxima
;*	Devuelve en la variable pos_max la posicion del motor para la cual la intensidad es maxima
;   Utiliza los registros r24,r25,rtn,max,X,Y

;*****************************************************************
calibracion:
            ldi Xl,low(cant_pasos)
            ldi Xh,high(cant_pasos)
            ld r25,X+;contador decremental
            ld r24,X
            clr Yh;contador incremental, da la posicion dl motor
            clr Yl;
            clr max
                        
cal:        
            rcall sensor_reset
            rcall sensor;me devuelve el valor procesado en rtn
                   
            cp max,rtn
            brcs actualizar_maximo
cont:       adiw Yh:Yl,1;incremento la posicion del vector
            rcall stepforth;avanzo un paso con el motor
            rcall delayY
            sbiw r25:r24,1;disminuyo el contador
            brne cal

            ret

actualizar_maximo:
            mov max,rtn
            ldi Xl,low(pos_max)
            ldi Xh,high(pos_max)
            st X+,Yh
            st X,Yl
            rjmp cont

;*****************************************************************
;*	"lectura"
;*	Rutina que realiza el espectro de una fuente de luz
;   Guarda los valores en la variable mediciones
;*	Toma los valores cada dieciseis pasos del motor
;*****************************************************************
lectura:
           MoveCalibraTo posMinLambdaHigh  
           rcall EEPROM_read 
           mov r25,rtn
           mov r3,rtn

           MoveCalibraTo posMinLambdaLow
           rcall EEPROM_read 
           mov r24,rtn;guardo en r1:r0 la cantidad de pasos que equivalen 
           mov r2,rtn
voy_al_comienzo:
           rcall stepforth
           rcall delayY
           sbiw r25:r24,1
           brne voy_al_comienzo
                 
          
           
                                
	   	     ldi Xl,low(cant_pasos)
           ldi Xh,high(cant_pasos)
           ld Yh,X+;guardo en y la cantidad de pasos
           ld Yl,X
           mov r1,Yh
           mov r0,Yl
           rcall resta
           mov Yh,r1
           mov Yl,r0

           MoveCalibraTo deltaPasoXlambdaHigh;  
           rcall EEPROM_read 
           mov r1,rtn
           MoveCalibraTo deltaPasoXlambdaLow
           rcall EEPROM_read 
           mov r0,rtn;guardo en r1:r0 la cantidad de pasos que equivalen a 1nm


           ldi Xl,low(mediciones)
           ldi Xh,high(mediciones)
sigo_midiendo: 
            mov r25,r1
            mov r24,r0
            rcall sensor_reset
            rcall sensor
            st X+,rtn
x_pasos:
            rcall stepforth
            rcall delayY
            sbiw Yh:Yl,1
            breq f
            sbiw r25:r24,1
            brne x_pasos
       	    rjmp sigo_midiendo 
f:      	ret

 ;*****************************************************************
;*	"conversion"
;*	Rutina que con los dos valores de lamba establece una equivalencia entre las posiciones del vector de mediciones y sus longitudes de onda
;*   Devuelve en rtn cuantas longitudes de onda equivalen a 16 pasos del motor
;  
;*	
;*****************************************************************
conversion:
           MoveCalibraTo lambda1High;el primer valor d lambda  
           rcall EEPROM_read 
           mov r1,rtn
           MoveCalibraTo lambda1Low
           rcall EEPROM_read 
           mov r0,rtn;guardo en r1:r0 el valor de lambda 1


           MoveCalibraTo lambda2High 
           rcall EEPROM_read 
           mov r3,rtn
           MoveCalibraTo lambda2Low
           rcall EEPROM_read
           mov r2,rtn;guardo en r3:r2 el valor de lambda 2

           rcall resta
           ;esta funcion resta los dos y me devuelve el valor absoluto en r1:r0
           
           ldi Xl,low(delta_lambda)
           ldi Xh,high(delta_lambda)
           
           st X+,r1;guardo delta_lambda
           st X,r0
            
           MoveCalibraTo calibra1High
           rcall EEPROM_read 
           mov r1,rtn
           MoveCalibraTo calibra1Low
           rcall EEPROM_read 
           mov r0,rtn;guardo en r1:r0 el valor de calibra 1

           MoveCalibraTo calibra2High 
           rcall EEPROM_read       
           mov  r3,rtn
           MoveCalibraTo calibra2Low
           rcall EEPROM_read       
           mov  r2,rtn;guardo en r3:r2 el valor de calibra 2
           
           rcall resta  
           ;me devuelve el valor absoluto en r1:r0
           ldi Xl,low(delta_paso)
           ldi Xh,high(delta_paso)
           st X+,r1
           st X,r0

           ;ahora hago la regla de 3 simple, tengo q dividir delta_paso/delta_lambda
            
           ldi Xl,low(delta_paso)
           ldi Xh,high(delta_paso)
           ld r1,X+
           ld r0,X
           ldi Xl,low(delta_lambda)
           ldi Xh,high(delta_lambda)
           ld r3,X+
           ld r2,X   
           rcall div16_16
           ;el resultado queda en  r3:r2
           MoveCalibraTo deltaPasoXlambdaHigh
           mov arg,r3
           rcall EEPROM_write
           MoveCalibraTo deltaPasoXlambdaLow
           mov arg,r2
           rcall EEPROM_write
           
           rcall buscoposmin


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

buscoposmin:
           MoveCalibraTo calibra1High
           rcall EEPROM_read 
           mov r6,rtn
           MoveCalibraTo calibra1Low
           rcall EEPROM_read 
           mov r7,rtn;guardo en r1:r0 el valor de calibra1
           
           MoveCalibraTo lambda1High;el primer valor d lambda  
           rcall EEPROM_read 
           mov r3,rtn
           MoveCalibraTo lambda1Low
           rcall EEPROM_read 
           mov r2,rtn;guardo en r1:r0 el valor de lambda 1         
           
           MoveCalibraTo deltaPasoXlambdaHigh
           rcall EEPROM_read
           mov r25,rtn
           MoveCalibraTo deltaPasoXlambdaLow
           rcall EEPROM_read
           mov r24,rtn 
           
comparacion:
           ldi tmp,0x01
           mov r1,tmp
           ldi tmp,0x90
           mov r0,tmp;
           ;r3:r2 contienen el valor d lambda1
           ;al que resto 400(0x0190)
           rcall resta
           ;el resultado me quedo en r1:r0
sigo:
           rcall sumar_a_si_mismo
           ;r25:r24 contiene el deltapasoporlambda
           sbiw r25:r24,1
comparo_p_alta:
           cpi  r25,0
           brne  sigo
           
comparo_p_baja:
           cpi r24,0
           brne sigo           
es_cero:  
          ; en r7:r6 tengo el valor de calibra1
          mov r3,r7
          mov r2,r6

          ;se lo resto a deltapasostotales
          rcall resta

          ;despues de restar lo guardo en eeprom
          MoveCalibraTo posMinLambdaHigh
          mov arg,r1
          rcall EEPROM_write
          MoveCalibraTo posMinLambdaLow
          mov arg,r0
          rcall EEPROM_write 
          ret            



sumar_a_si_mismo:
         add r0,r0
         adc r1,r1
         ret 

                               