; Alvin Baldemeca
; TCES 430, UWT
; Homework 4, HW4_Part1.asm
; Prof. Sheng
;
; This program implements the following C code to a PIC18 assembly code.
;
; int find_max (int *ia, unsigned char cnt) {
; int k;
; k = 0;
; while (cnt != 0) {
;   if (*ia > k) k=*ia;
;   ia++; cnt--;
;  }
; return (k);
; }
;
; Note: You can change the values in array_of_int however please DO NOT change
; the size of this array since the size is hard coded.  Also the variable max is
; in data address 10B and 10A which should contain the max value at the end of
; this program.



#include <P18F4520.inc>
#define f 1

 cblock 0
    int_array :18, myk :2, max :2, myCnt, temp
 endc
    
    org 0
    goto Main

    org 0x200

; int numbers[9] = {65, -320, 43, 98, 7, 87, 4095, 23, 10};
array_of_int dw D'65', H'FEC0', D'43', D'98', D'7', D'87', D'4095', D'23', D'10';

Main
; We are going to retrieve data sotred in program memory to data memory placing
; array_of_int at data address 000 to 101
    movlw D'18'  ;
    movwf temp, f

    movlw upper array_of_int
    movwf TBLPTRU
    movlw high array_of_int
    movwf TBLPTRH
    movlw low array_of_int
    movwf TBLPTRL
    lfsr FSR0, int_array
begin_array_move
    
    tblrd*+ ;use table read to get byte
    movf TABLAT, w ; move to w
    movwf POSTINC0 ;store byte to S1, FSR0++
    dcfsnz temp
    bra end_array_move
    bra begin_array_move

end_array_move

; Subroutine call to find_max
    rcall find_max

; store the return value to max
; max = find_max(myk, myCnt)
    movff myk, max
    movff myk + 1, max + 1

    bra Finish ; We are done!


;This is a subroutine
find_max
    movlw D'9'
    movwf myCnt ; myCnt = 9 which is the size of the int[] array
    lfsr FSR0, int_array ; use pointer to iterate through the array
    clrf myk ; k = 0, initialize k to be 0, clears the first byte
    clrf myk + 1 ; clears the second byte

While_start ;start of the while loop

;Test if *ia > k
    movf POSTINC0, w
    subwf myk, w
    movf INDF0, w
    subwfb myk + 1, w
    bov ov_test
    bn if_true
if_false
    movf POSTINC0, w   ; Used to increment the pointer
    dcfsnz myCnt       ; cnt--
    bra Return_to_Main ; End of while loop
    bra While_start    ; loop if cnt != 0;

ov_test                ; Overflow test for *ia > k
    bnn if_true;
    bra if_false;
    
if_true                       ; if *ia > k,
    movff POSTDEC0 , myk + 1  ; k = *ia
    movff POSTINC0, myk
    movf  POSTINC0, w         ; Used to synchronize the pointer ia
    bra While_start

Return_to_Main
    return ; return call, could not return int since WREG is only a byte wide.


Finish
    goto Finish

    end







