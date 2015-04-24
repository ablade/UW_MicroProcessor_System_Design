; part3.asm
; Alvin Baldemeca
; TCES 430 11/14/2013
; UWT, Prof. Sheng
; Midterm
;
; The answer to the question m mod n is stored in my_temp.  This program
; calculates mod(m,n).


#include <p18f4520.inc>

#define f 1

; my_count holds the value of integer division.  my_tem holds the answer to
; the m mod n at the end of this program.
 CBLOCK 0x000
  my_m : 2, my_n : 2, my_count : 2, my_temp : 2
 ENDC

; We can set the values for m and n here we set m = 2580 and n to 276 so
; m mod n = 96 which is 0x0060 in hex and is the answer at the end of the
; program
LOW_M equ 0x14  ;0x07 <- test values
HIGH_M equ 0x0A ;0x00 <- test values
LOW_N equ 0x14  ;0x02 <- test values
HIGH_N equ 0x01 ;0x00 <- test values

	ORG 0
	goto Main    


	org 0x0200

Main

;We initialize m to the value we give it in HIGH_M and LOW_M
    movlw LOW_M
    movwf my_m, f
    movlw HIGH_M
    movwf my_m + 1

;We initialize m to the value we give it in HIGH_N and LOW_N
    movlw LOW_N
    movwf my_n, f
    movlw HIGH_N
    movwf my_n + 1

;Use temp to hold the value of m
    movff my_m, my_temp
    movff my_m+1, my_temp+1

mod_loop
    movf my_n, W            ;////////////////////////////////////////////////
    subwf my_temp, f        ;// We check to see if n > m
    movf my_n+1, W          ;//
    subwfb my_temp + 1, f   ;////////////////////////////////////////////////
    bnc end_mod             ; exit on no carry
    infsnz my_count         ;
    incf my_count +1        ; increase the integer division count
    bra mod_loop            ; keep subtracting if m > n

end_mod

  ; We need to add n to the temp to get the remainder or mod value
    movf my_n, W
    addwf my_temp, f
    movf my_n+1, W
    addwfc my_temp+1,f

loop_forever           ;keep the micro controller busy.
    bra loop_forever

    halt
	end


