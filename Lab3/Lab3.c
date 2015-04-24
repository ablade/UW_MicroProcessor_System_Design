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
 * This function stores a string into a char[64] called buffer.
 * @param progString the pointer to the string or char* to load in to the buffer
 */
void retrieve_word(const char* progString){

    int i = 0;
    while(progString[i] != 0)
    {
        buffer[i] = progString[i];
        i++;
    }
    buffer[i] = 0;
}

/**
 * This function prints out to the UART I/O a message containg the word to be
 * printed out and the length of the word
 * @param  -string the char pointer to the string or char[0]
 * @return - returns 0 if printf was successfully executed.
 */
int count_and_print(char* string)
{
    int i = 0;
    while(string[i]!= 0)
    {
        i++;
    }
    length = i;
    return printf("The length of \" %s \" is %i\n", string, length);
}

/**
 * The main function of the program calls on two helper functions.  One function
 * retrives a string and stores it into a buffer.  The other function counts the
 * number of characters in the word and prints out the word.
 * @param argc -the number of command line arguments (not used)
 * @param argv -the pointer to the command line arguments (not used)
 * @return -0 on success
 */
int main(int argc, char** argv) {

    SPEN = 1;
    TXEN = 1;
    const char STRING1[12] = "Hello World\0";
    const char STRING2[15] = "This is a test";
    const char* STRING3 = "Hi there";
    const char* STRING4 = "Alvin";
    const char* STRING5 = "Edward";
    
        retrieve_word(STRING1);
        count_and_print(buffer);

        retrieve_word(STRING2);
        count_and_print(buffer);

        retrieve_word(STRING3);
        count_and_print(buffer);

        retrieve_word(STRING4);
        count_and_print(buffer);

        retrieve_word(STRING5);
        count_and_print(buffer);
   
    while(1);
    return (EXIT_SUCCESS);
}