/* include/expand.h - Variable expansion prototypes */

#ifndef EXPAND_H
#define EXPAND_H

#include "shell.h"

/* Expand variables in string */
char *expand_string(char *str, shell_state_t *state);

/* Get environment variable */
char *get_env_var(char *name, shell_state_t *state);

/* Set environment variable */
void set_env_var(char *name, char *value, shell_state_t *state);

/* Unset environment variable */
void unset_env_var(char *name, shell_state_t *state);

#endif /* EXPAND_H */