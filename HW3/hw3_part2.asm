; hw3_part2.asm
;
; Alvin Baldemeca
; UWT TCES 430
; Homework 3 part 2
; 10/24/2013
; Prof. Sheng
;
;
; This assembly code is written for the PIC18F4520 micro-controller.  We use a
; macro to insert a delay.  The main code moves 0001b to register 0x000 (myi)
; it then shifts the value to the left until its value is 1000b with a 5ms delay
; after each shift to the left.


#include <p18f4520.inc>
#define F 1
 cblock 0x000
    myi, mya, myb
 endc

; This macro should delay for 5 ms for a 4MHz clock the calculations are as 
; follows :
; For 4MHZ clock we can execute 1M instructions since each instruction cycle 
; takes 4 clock cycles.  We therefore need 5,000 instruction cycles to execute
; in order to get a 5ms delay.  The first four instructions of this macro take
; up 4 instruction cycle.  The next four takes up (230 + 256 + 256 + 256)*5 =
; 4995 the bra takes 1 for a total of 5000 instruction cycles
delay_5ms macro regd1, regd2
    movlw	0xE6  ; 231 in decimal
	movwf	regd1
	movlw	0x04
	movwf	regd2
; These four instructions take up 5 instruction cycles,
; 2 for goto and i for decfsz
	decfsz	regd1, f
	goto	$ + 6
	decfsz	regd2, f
	goto	$ - 8
			;1 cycles
	bra	$ + 2
    endm

reset_vector code 0x00
    bra Main

    code 0x200
; The main code moves 0001b to register 0x000 (myi) it then shifts the value to
; the left until its value is 1000b with a 5ms delay after each shift.
Main
    movlw B'0001'
    movwf myi, A
loop0
    RLNCF myi, F
    delay_5ms mya, myb
    movlw B'1000'
    CPFSEQ myi
    bra loop0

Finish halt
    end