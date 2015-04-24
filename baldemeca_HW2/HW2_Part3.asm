; HW2_Part3.asm
; Alvin Baldemeca
; TCES 430 10/10/2013
; UWT, Prof. Sheng
;
;  Assingment 2 Problem 3 asked for the following:
; 3. (50%) The following C code counts the number of ?1? bits in an unsigned,
; one-byte value j and returns the answer in k. Convert this to PIC18 assembly
; code, and assign initial value to j using code instead of manually assigning
; value in MPLAB/MPLAB X IDE.
;
;   k=0;
;   for (i=0; i!=8; i++)
;   {
;       if (j&0x01)
;       {
;           k++;
;       }
;       j=j>>1;
;   }

#include <p18f4520.inc>		;#include derective includes additional code. This
                            ; is a header file which defines configurations,
                            ;registers, and other useful bits of information for
                            ;the PIC18F4520 microcontroller

#define F 1 ;This is used to represent data register and not the working register (WREG)

;Reserve memory space for the given name
 CBLOCK 0x100
  myI, myJ, myK
 ENDC

;Assembly directive to associate the variable with the value
VAL_OF_I equ D'0' ;D'0'
VAL_OF_J equ H'F1'  ;H'F8'
VAL_OF_K equ D'0'

;The ORG directive is used to set the program or register address.  In this case
;we put the command goto Main in address 0
	ORG 0
	goto Main    ;point the reset vector to the start of our program

;We assemble the following program in the memory location 0x0200
	org 0x0200

Main

    ; Initialize registers
    movlw VAL_OF_J    ; move VAL_OF_J to working register (WREG)
    movwf myJ, F      ; move the value in the WREG to myJ
    movlw VAL_OF_K    ; move VAL_OF_K to WREG, WREG = 0x00
    movwf myK, F      ; k=0, move 0x00 to myK
    movwf myI, F      ; i=0, move 0x00 to myI

FOR_LOOP
    movlw 8           ; move 8 (literal) to WREG
FOR_TEST  subwf myI, W           ; Test condition in for loop
                                 ; "for( ; i!=8; )"
    bz END_LOOP                  ; if(i==8) we are done with the for loop
    
    movf myJ, W
    andlw 0x01                  ; (j & 0x01)
    bz ROTATE                   ; if(j&0x01 == 0) go to Rotate to shift left
    incf myK, F                 ; else k++

ROTATE
    bcf STATUS, 0       ; Set carry flag to 0 we can also use "addlw 0x00" which was used be for "bcf" with success
    rrcf myJ, F         ; Shift right j>>1
                
    incf myI, F         ; i++ increment our for loop iterator
    bra FOR_LOOP        ; loop back

END_LOOP

; Unsure if this was the repeated instructions ask for in the howework
; description.  Please uncomment the NO_OP loop below if required.
;NO_OP
;    addlw 0
;    addlw 0
;    addlw 0
;    bra NO_OP

 halt
	end








