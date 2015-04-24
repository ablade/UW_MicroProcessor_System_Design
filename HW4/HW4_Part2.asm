; Alvin Baldemeca
; TCES 430, UWT
; Homework 4, HW4_Part2.asm
; Prof. Sheng
;
; This program is suppose to perfrom the following requirements stated below:
;
; Write a PIC18 subroutine that will initialize the contents of an integer array
; stored in data memory with the contents of an integer array stored in program
; memory. Assume FSR0 points to the integer array in data memory, TBLPTR to the
; integer array in program memory, and the W register contains the number of
; integers to be copied. You will probably need to use another temporary memory
; location to track the number of integers that have been copied from program
; memory to data memory.
; For this problem, assume the variable ptr1 contains the three-byte address of
; the array in program memory and ptr2 contains the two-byte address of the
; array in data memory. Another char variable, cnt, has been reserved for the
; remaining count of integers to be transferred.

#include <P18F4520.inc>
#define f 1

 cblock 000
    ptr2 :18, cnt
 endc

    org 0
    goto Main

    org 0x200

ptr1 dw D'65', H'FEC0', D'43', D'98', D'7', D'87', D'4095', D'23', H'FFFF';

Main
    movlw D'18'
    rcall mov_P_to_D
    bra Finish

mov_P_to_D
    movwf cnt, f     ;WREG -> cnt, set count to the size of program mem. to transfer to data mem

    ;Set table pointer to point to ptr1 address in program memory
    movlw upper ptr1
    movwf TBLPTRU
    movlw high ptr1
    movwf TBLPTRH
    movlw low ptr1
    movwf TBLPTRL

    ;Set data pointer 1 to point to prt2 address in data memory
    lfsr FSR0, ptr2
begin_array_move   
    tblrd*+                 ; use table read to get byte
    movf TABLAT, w          ; move to w
    movwf POSTINC0          ; store byte to ptr2, FSR0++
    dcfsnz cnt              ; decrement cnt skip next instruction if result != 0
    bra end_array_move      ; If cnt == 0 we are done moving data.
    bra begin_array_move

end_array_move
    return          ; return from subroutine

Finish
    goto Finish    ; busy loop used for debugger on simulator for MPLABX
    end


