/* include/parser.h - Parser function prototypes */

#ifndef PARSER_H
#define PARSER_H

#include "shell.h"

/* Main parsing function */
command_t *parse_command(char *input, shell_state_t *state);

/* Cleanup function */
void free_command(command_t *cmd);

/* Variable expansion for parser */
char *expand_variables(char *arg, shell_state_t *state);

#endif /* PARSER_H */