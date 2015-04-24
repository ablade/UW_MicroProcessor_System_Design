/*
 * File:   Lab3_mod.c
 * Author: Alvin Baldemeca, Edward Bassan
 * UWT TCES 430
 * Lab 3
 * Prof. Sheng
 * Created on October 22, 2013, 11:28 AM
 */

#include <stdio.h>
#include <stdlib.h>
#include <xc.h>
#include <p18f4520.h>
#pragma config WDT=OFF
#define MAX_LENGTH 64

static char buffer[MAX_LENGTH];
static int length;

void putch(char c)
{
    while(!TRMT);
    TXREG = c;
}

/**
 * Ths is a helper function for the main function.  It is a subroutine that
 * retrieves the string stored in the program memory and loads it into data
 * memory and counts how many character/s the word is.
 * @param progString the pointer to the program string
 */
void get_count_print(const char* progString){

    int i = 0;
    while(progString[i] != 0)
    {
        buffer[i] = progString[i];
        i++;
    }
    buffer[i] = 0;
    length = i;
    printf("The length of the word \" %s \" is %i\n", buffer, length);
}

/**
 * The main function calls a subroutine.  The subroutine places each char in the
 * string in a buffer, it counts the number of characters and prints the word
 * and the number of characters in the word.
 * @param argc the number of command line arguments (not used)
 * @param argv the pointer to the command line arguments (not used)
 * @return
 */
int main(int argc, char** argv) {

    SPEN = 1;
    TXEN = 1;
    const char* STRING1 = "Hello World";
    const char* STRING2 = "This is a test";
    const char* STRING3 = "Hi there";
    const char* STRING4 = "Alvin";
    const char* STRING5 = "Edward";
  
    get_count_print(STRING1);
    get_count_print(STRING2);
    get_count_print(STRING3);
    get_count_print(STRING4);
    get_count_print(STRING5);
    
    /*
     * This while loop is needed for the simulator because so that it does not
     * loop indefinately
     */
    while(1); 
    return (EXIT_SUCCESS);
}