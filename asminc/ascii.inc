; ********************************************************
; * Number conversion routines, Version 0.1 January 2002 *
; * (C)2002 by http://www.avr-asm-tutorial.net           *
; ********************************************************

;*****************************************************************
;*****************************************************************
;*				Rutinas encontradas online
;*				Adaptadas para poder emplear
;*				en codigo del EFC
;*****************************************************************
;*****************************************************************

; Bin2ToAsc5
; ==========
; converts a 16-bit-binary to a 5 digit ASCII-coded decimal
; In: 16-bit-binary in rBin1H:L, Z points to the highest
;   of 5 ASCII digits, where the result goes to
; Out: Z points to the beginning of the ASCII string, lea-
;   ding zeros are filled with blanks
; Used registers: rBin1H:L (content is not changed),
;   rBin2H:L (content is changed), tmp
; Called subroutines: Bin2ToBcd5
;
Bin2ToAsc5:
	rcall Bin2ToBcd5 ; convert binary to BCD
	ldi tmp,4 ; Counter is 4 leading digits
	mov rBin2L,tmp
Bin2ToAsc5a:
	ld tmp,z ; read a BCD digit
	tst tmp ; check if leading zero
	brne Bin2ToAsc5b ; No, found digit >0
	ldi tmp,' ' ; overwrite with blank
	st z+,tmp ; store and set to next position
	dec rBin2L ; decrement counter
	brne Bin2ToAsc5a ; further leading blanks
	ld tmp,z ; Read the last BCD
Bin2ToAsc5b:
	inc rBin2L ; one more char
Bin2ToAsc5c:
	subi tmp,-'0' ; Add ASCII-0
	st z+,tmp ; store and inc pointer
	ld tmp,z ; read next char
	dec rBin2L ; more chars?
	brne Bin2ToAsc5c ; yes, go on
	sbiw ZL,5 ; Pointer to beginning of the BCD
	ret ; done
;


;Binary to ASCII without leading blanks

; Bin2ToAsc
; =========
; converts a 16-bit-binary to a 5-digit ASCII coded decimal,
;   the pointer points to the first significant digit of the
;   decimal, returns the number of digits
; In: 16-bit-binary in rBin1H:L, Z points to first digit of
;   the ASCII decimal (requires 5 digits buffer space, even
;   if the number is smaller!)
; Out: Z points to the first significant digit of the ASCII
;   decimal, rBin2L has the number of characters (1..5)
; Used registers: rBin1H:L (unchanged), rBin2H (changed),
;   rBin2L (result, length of number), tmp
; Called subroutines: Bin2ToBcd5, Bin2ToAsc5
;
Bin2ToAsc:
	rcall Bin2ToAsc5 ; Convert binary to ASCII
	ldi tmp,6 ; Counter is 6
	mov rBin2L,tmp
Bin2ToAsca:
	dec rBin2L ; decrement counter
	ld tmp,z+ ; read char and inc pointer
	cpi tmp,' ' ; was a blank?
	breq Bin2ToAsca ; Yes, was a blank
	sbiw ZL,1 ; one char backwards
	ret ; done
;


;From binary to BCD

; Bin2ToBcd5
; ==========
; converts a 16-bit-binary to a 5-digit-BCD
; In: 16-bit-binary in rBin1H:L, Z points to first digit
;   where the result goes to
; Out: 5-digit-BCD, Z points to first BCD-digit
; Used registers: rBin1H:L (unchanged), rBin2H:L (changed),
;   tmp
; Called subroutines: Bin2ToDigit
;
Bin2ToBcd5:
	push rBin1H ; Save number
	push rBin1L
	ldi tmp,HIGH(10000) ; Start with tenthousands
	mov rBin2H,tmp
	ldi tmp,LOW(10000)
	mov rBin2L,tmp
	rcall Bin2ToDigit ; Calculate digit
	ldi tmp,HIGH(1000) ; Next with thousands
	mov rBin2H,tmp
	ldi tmp,LOW(1000)
	mov rBin2L,tmp
	rcall Bin2ToDigit ; Calculate digit
	ldi tmp,HIGH(100) ; Next  with hundreds
	mov rBin2H,tmp
	ldi tmp,LOW(100)
	mov rBin2L,tmp
	rcall Bin2ToDigit ; Calculate digit
	ldi tmp,HIGH(10) ; Next with tens
	mov rBin2H,tmp
	ldi tmp,LOW(10)
	mov rBin2L,tmp
	rcall Bin2ToDigit ; Calculate digit
	st z,rBin1L ; Remainder are ones
	sbiw ZL,4 ; Put pointer to first BCD
	pop rBin1L ; Restore original binary
	pop rBin1H
	ret ; and return
;
; Bin2ToDigit
; ===========
; converts one decimal digit by continued subraction of a
;   binary coded decimal
; Used by: Bin2ToBcd5, Bin2ToAsc5, Bin2ToAsc
; In: 16-bit-binary in rBin1H:L, binary coded decimal in
;   rBin2H:L, Z points to current BCD digit
; Out: Result in Z, Z incremented
; Used registers: rBin1H:L (holds remainder of the binary),
;   rBin2H:L (unchanged), tmp
; Called subroutines: -
;
Bin2ToDigit:
	clr tmp ; digit count is zero
Bin2ToDigita:
	cp rBin1H,rBin2H ; Number bigger than decimal?
	brcs Bin2ToDigitc ; MSB smaller than decimal
	brne Bin2ToDigitb ; MSB bigger than decimal
	cp rBin1L,rBin2L ; LSB bigger or equal decimal
	brcs Bin2ToDigitc ; LSB smaller than decimal
Bin2ToDigitb:
	sub rBin1L,rBin2L ; Subtract LSB decimal
	sbc rBin1H,rBin2H ; Subtract MSB decimal
	inc tmp ; Increment digit count
	rjmp Bin2ToDigita ; Next loop
Bin2ToDigitc:
	st z+,tmp ; Save digit and increment
	ret ; done
