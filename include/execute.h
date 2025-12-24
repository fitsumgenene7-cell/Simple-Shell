#ifndef EXECUTE_H
#define EXECUTE_H

#include "shell.h"

int execute_command(command_t *cmd, shell_state_t *state);

int execute_external(command_t *cmd, shell_state_t *state);

int handle_redirection(command_t *cmd);

#endif 