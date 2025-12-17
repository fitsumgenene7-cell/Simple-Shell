/* include/execute.h - Execution function prototypes */

#ifndef EXECUTE_H
#define EXECUTE_H

#include "shell.h"

/* Execute a command */
int execute_command(command_t *cmd, shell_state_t *state);

/* Execute external command */
int execute_external(command_t *cmd, shell_state_t *state);

/* Handle redirection */
int handle_redirection(command_t *cmd);

#endif /* EXECUTE_H */