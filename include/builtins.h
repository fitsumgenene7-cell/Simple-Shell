/* include/builtins.h - Built-in command prototypes */

#ifndef BUILTINS_H
#define BUILTINS_H

#include "shell.h"

/* Check if command is built-in */
int is_builtin(char *cmd);

/* Built-in implementations */
int builtin_exit(command_t *cmd, shell_state_t *state);
int builtin_cd(command_t *cmd, shell_state_t *state);
int builtin_env(command_t *cmd, shell_state_t *state);
int builtin_setenv(command_t *cmd, shell_state_t *state);
int builtin_unsetenv(command_t *cmd, shell_state_t *state);
int builtin_alias(command_t *cmd, shell_state_t *state);
int builtin_path(command_t *cmd, shell_state_t *state);

#endif /* BUILTINS_H */