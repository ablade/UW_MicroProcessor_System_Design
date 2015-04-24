/*
 * File:   Lab2.c
 * Author: Alvin Baldemeca, Edward Bassan
 * UWT TCES 430
 * Lab 2
 * Prof. Sheng
 * Created on October 15, 2013, 10:35 AM
 */
#include <p18f4520.h>
#pragma config WDT=OFF
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

/**
 * This is a helper function for main() which determines if the function 
 * x^3 + 2x^2 - 10x + 40 = 0 crosses the y-axis.
 * @param thX the value to substitue x in the equation
 * @return returns 1 if x is a root of the function 0 otherwise.
 */
int findx(float thX)
{
    float myX = 0.00;
    float myNextX;
    float theX = thX;
    myX = pow(theX,3) + 2*pow(theX,2) - 10*theX + 40;
    theX = theX + 0.01;
    myNextX = pow(theX,3) + 2*pow(theX,2) - 10*theX + 40;
    printf(" and y is = %f\n", myX);
    return ((myX < 0 && myNextX > 0) ||( myX > 0 && myNextX < 0) ? 1 : 0);
}

/**
 * This functions enables the simulator to output the UART to a window for the
 * simulator on MPLABX IDE
 * @param c the character to print.
 */
void putch(char c)
{
    while(!TRMT);
    TXREG = c;
}

/**
 * This solves for the equation x^3 + 2x^2 - 10x + 40 = 0.  It prints out the
 * value of x and y tested and any real solutions found. 
 * @param argc number of command line arguments(not used)
 * @param argv the pointer to the string argument values (not used) 
 * @return returns 0 on success
 */
int main(int argc, char** argv) {

    SPEN = 1;
    TXEN = 1;

    float x = -10.0;
    float roots[10];
    int y = 0;
    int i = 0;
    while(x <=10.0){
        printf("x = %f ", x);
        y = findx(x);
        if(y)
        {
            x = x + 0.005;
            roots[i] = x;
            i++;
            printf("Solution x = %f\n", x);
            //The solution when this was ran is x = -5.310424            
        }
        x = x + 0.01;
    }

    printf("The solution/s are x = ");
    int j = 0;
    if(roots[0]!= NULL){
        for( j=0; j < i; j++ )
        {
            printf("%f, ", roots[j]);
        }
        printf("\n");
    }else{
        printf("No roots found\n");
    }

   //While loop is needed when running the simulator otherwise the program
   // starts over and executs.
   while(1);

    return (EXIT_SUCCESS);
}



