#include "../include/shell.h"

/* Variable expansion stub - to be implemented */
/* For now, just returns a copy of the string */
char *expand_variables(char *arg, shell_state_t *state)
{
    (void)state; /* Unused parameter for now */
    return my_strdup(arg);
}
