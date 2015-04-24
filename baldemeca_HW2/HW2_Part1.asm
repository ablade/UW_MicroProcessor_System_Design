; HW2_Part1.asm
; Alvin Baldemeca
; TCES 430 10/10/2013
; UWT, Prof. Sheng
;
; Assingment 2 Problem 1 asked for the following:
; 1. (20%) Write PIC18 assembly code that begins with an unsigned, one-byte
; value, p, and executes several program lines, subprogA, if p is greater than
; another unsigned, one-byte value, q; executes lines, subprobB, if p is less
; than q; and executes lines, subprogC, if p equals q. Assume both p and q are
; in ACCESS data memory, and assign initial values to p and q using code instead
; of manually assigning values in MPLAB/MPLAB X IDE.

#include <p18f4520.inc>		;#include derective includes additional code. This
                            ; is a header file which defines configurations,
                            ;registers, and other useful bits of information for
                            ;the PIC18F4520 microcontroller

;first we want to use a macro to hold 4 variables myA myB myC and myS
 CBLOCK 0x100
  myP, myQ, myA
 ENDC

;We can change the random number that we picked in this block of code.  These are
;two bytes unsinged ints little endian so A is LSB and A1 is MSB same for B, C and S
VAL_OF_P equ D'10'
VAL_OF_Q equ D'11'
;The ORG directive is used to set the program or register address.  In this case
;we put the command goto Main in address 0
	ORG 0
	goto Main    ;point the reset vector to the start of our program

;We assemble the following program in the memory location 0x0200
	org 0x0200

Main
;We initialize our unsingned ints myA, myB, myC
    movlw VAL_OF_P               ; move value of A to WREG (working rigister)
    movwf myP                    ; move from WREG to myA
    movlw VAL_OF_Q              ; move the MSB byte block to the WREG
    movwf myQ                ; move the contents of the WREG to the MSB of myA

    subwf myP, W                         ;myP - myQ
    bz PROG_C                    ; Branch to PROG_C if myP == myQ
    bn PROG_B                    ; Branch to PROG_B if myP < myQ
;Some instructions here for Program A
    movlw 0xAA
    movwf myA
    bra FINISH

PROG_B
;Put instructions here for Program B
    movlw 0xBB
    movwf myA
    bra FINISH

PROG_C
;Put instructions here for Program C
    movlw 0xCC
    movwf myA

FINISH
; Unsure if this was the repeated instructions ask for in the howework
; description.  Please uncomment the NO_OP loop below if required.
;NO_OP
;    addlw 0
;    addlw 0
;    addlw 0
;    bra NO_OP
    halt
	end


