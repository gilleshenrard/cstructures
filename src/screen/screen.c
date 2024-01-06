/**
 * @file screen.c
 * @brief Implements screen-specific manipulations
 * @author Gilles Henrard
 * @date 06/01/2024
 */
#include "screen.h"
#include <stdio.h>
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

/**
 * @brief Print an error message in bold red on screen
 * 
 * @param format Format to apply to the message
 * @param ... Additional parameters used by the print function
 */
void printError(const char* format, ...){
    char final_msg[SZLINE] = {0};
    va_list arg;

    va_start(arg, format);
        vsprintf_s(final_msg, SZLINE, format, arg);
        fprintf_s(stderr, "\033[%d;%dm%s\033[0m\n", BOLD, RED, final_msg);
    va_end(arg);
}
