/* 
 * File:   hello.c
 * Author: Alvin Baldemeca, Edward Bassan
 * UWT TCES 430
 * Lab 2
 * Prof. Sheng
 * Created on October 15, 2013, 10:35 AM
 *
 * This program is a simple "Hello World!" program for the PIC 18F4520 micro-
 * controller.  Please link the MPASMX linker file 18f4520_g.lkr to this file,
 * also this program was compiled using Microchip's XC8 compiler.  This program
 * prints the output to the UART 1 Output window.
 */

#include <p18f4520.h>
#pragma config WDT=OFF
#include <stdio.h>
#include <stdlib.h>


void putch(char c)
{
    while(!TRMT);
    TXREG = c;
}

/**
 * The main fucntion prints "Hello Wolrd"
 * @param argc - the number of command line argumens (not used)
 * @param argv - the pointer to the command line arguments (not used)
 * @return
 */
int main(int argc, char** argv){

    SPEN = 1;
    TXEN = 1;
    printf("Hello World!\n");
    while(1); //Needed for the simulation of the program to keep the simulator
              //busy.
    return (EXIT_SUCCESS);
}

