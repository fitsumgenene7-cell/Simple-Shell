#include "../include/shell.h"

char *expand_variables(char *arg, shell_state_t *state)
{
    (void)state; /* Unused parameter for now */
    return my_strdup(arg);
}
