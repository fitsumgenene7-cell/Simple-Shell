/* src/error.c - Unified error handling */

#include "../include/shell.h"

void print_error(void)
{
    fprintf(stderr, ERROR_MSG);
}