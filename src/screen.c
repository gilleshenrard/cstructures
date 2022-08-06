/*
** screen.c
** Library providing an error printing function
** ---------------------------------------------------
** Made by Gilles Henrard
** Last modified : 06/08/2022
*/
#include "screen.h"
#include<stdio.h>
#include <stdarg.h>

#define RESET   0
#define RED     31
#define GREEN   32
#define YELLOW  33
#define BLUE    34
#define MAGENTA 35
#define CYAN    36

#define NORMAL  0
#define BOLD    1

#define SZLINE  128

/************************************************************/
/*  I : Error message to print                              */
/*      Parameters to add in the message                    */
/*  P : Print an error message                              */
/*  O : /                                                   */
/************************************************************/
void printError(char* format, ...){
    char final_msg[SZLINE] = {0};
    va_list arg;

    va_start(arg, format);
        vsprintf(final_msg, format, arg);
        fprintf(stderr, "\033[%d;%dm%s\033[0m\n", BOLD, RED, final_msg);
    va_end(arg);
}
