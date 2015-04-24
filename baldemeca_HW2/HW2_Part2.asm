; HW2_Part2.asm
; Alvin Baldemeca
; TCES 430 10/10/2013
; UWT, Prof. Sheng
;
; Assingment 2 Problem 2 asked for the following:
; 2. (30%) Write PIC18 assembly code that begins with an unsigned, one-byte
; value, j, and subtracts an integer value from j, beginning with one and
; incrementing the subtracted value by one each time. The loop is halted when
; the result is zero or negative. Execute subprogA if zero is reached and
; subprogB if a negative result is reached. Do the comparison by checking with
; the Carry flag only. Assume j is in ACCESS data memory, and assign initial
; values to j using code instead of manually assigning value in MPLAB/MPLAB X
; IDE.

#include <p18f4520.inc>		;#include derective includes additional code. This
                            ; is a header file which defines configurations,
                            ;registers, and other useful bits of information for
                            ;the PIC18F4520 microcontroller

#define F 1 ;This is used to represent data register and not the working register (WREG)


;first we want to use a macro to hold 4 variables myA myB myC and myS
 CBLOCK 0x100
  myI, myJ, myK
 ENDC

;We can change the random number that we picked in this block of code.  These are
;two bytes unsinged ints little endian so A is LSB and A1 is MSB same for B, C and S
VAL_OF_I equ D'1'
VAL_OF_J equ D'5'  ;H'F8'
;The ORG directive is used to set the program or register address.  In this case
;we put the command goto Main in address 0
	ORG 0
	goto Main    ;point the reset vector to the start of our program

;We assemble the following program in the memory location 0x0200
	org 0x0200

Main

    movlw VAL_OF_I               ; move value of A to WREG (working rigister)
    movwf myI                    ; move from WREG to myA
    movlw VAL_OF_J              ; move the MSB byte block to the WREG
    movwf myJ                ; move the contents of the WREG to the MSB of myA

LOOP
    
    movf myI, W
    subwf myJ, W                 ; myJ - myI
    bz PROG_A                    ; Branch to PROG_A if myJ == myI
    bn PROG_B                    ; Branch to PROG_B if N flag is set
    incf myI, F                  ; increment myI
    bra LOOP

PROG_A
;Put instructions here for Program A
    movlw 0xAA
    movwf myK
    bra FINISH


PROG_B
;Put instructions here for Program B
    movlw 0xBB
    movwf myK

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





