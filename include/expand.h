#ifndef EXPAND_H
#define EXPAND_H

#include "shell.h"

char *expand_string(char *str, shell_state_t *state);

char *get_env_var(char *name, shell_state_t *state);

void set_env_var(char *name, char *value, shell_state_t *state);

void unset_env_var(char *name, shell_state_t *state);

#endif 