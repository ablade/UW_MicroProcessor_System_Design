; hw3_part1.asm
;
; Alvin Baldemeca
; UWT TCES 430
; Homework 3 part 1
; 10/24/2013
; Prof. Sheng
;
; This assembly code should perform the following C program:
;
; i = 1
; j = 0
; k = -1
; a = values used are between 0x08 and 0x0F
; b = values used are between 0x08 and 0x0F

; while (i > j)
; {
;   i = i + a ? 2 * j;
;   if (j >= k)
;   {
;       i = i + 2;
;       k = k ? b + 2 * j;
;   }
;   j++;
; }
;
;
; Different values were attempted here are a few results of the final values
; For A > B:  A = 0x0E and B = 0x09 i = 0x09 j = 0x10 k = 0x17
; For B > A:  A = 0x09 and B = 0x0E i = 0x01 j = 0x0C k = 0xDB
; For A = B:  A = 0x0A and B = 0x0A i = 0x01 j = 0x0D k = 0x19

#include <p18F4520.inc>

 cblock 0x000
    myi, myj, myk, mya, myb
 endc

VAL_OF_I equ D'1'
VAL_OF_J equ D'0'
VAL_OF_K equ 0xFF ; k = -1
VAL_OF_A equ 0x09
VAL_OF_B equ 0x0E
    org 0
    goto Main

    org 0x200
Main
;This block initializes the registers with a given value
    movlw 1
    movwf myi, A
    movlw 0
    movwf myj, A
    movlw 0xFF
    movwf myk, A
    movlw VAL_OF_A
    movwf mya, A
    movlw VAL_OF_B 
    movwf myb, A


while_i_gt_j
    ; test if i > j
    movf myi, W
    subwf myj, W
    bov TEST1    ; If there is overflow we must test the N Status bit
    bnn FINISH   ;Since these are signed numbers if there is no overflow and
                 ; j - i is not negative then j == i we must exit the while loop
while_true
    ; i = i + a - 2 * j
    movf myj, 0 ; 2 * j is j + j -> wreg
    addwf myj,0 ; Note we could have used mulwf mnemonic
    subwf mya, 0; a - wreg -> wreg
    addwf myi,1; wreg + i -> i
    

   ; if(j >=k)
   movf myk, W; k -> wreg
   subwf myj, W; j - wreg ->wreg
   bov TEST2
   bn if_false
if_true
; i = i + 2
    movlw 2
    addwf myi, 1

; k = k - b + 2 * j
    movf myj, 0 ; 2 * j is j + j -> wreg
    addwf myj,0
    addwf myk, 1
    movf myb, 0 ; b -> wreg
    subwf myk,1; k = k - wrig;

if_false
    incf myj
    goto while_i_gt_j

TEST1
    bnn while_true
    bra FINISH

TEST2
    bn if_true
    bra if_false

FINISH
 halt
    end



