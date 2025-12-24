/* include/path.h - PATH function prototypes */

#ifndef PATH_H
#define PATH_H

#include "shell.h"
void init_path(shell_state_t *state);

/* Find command in PATH */
char *find_command(char *cmd, shell_state_t *state);

/* Set new PATH */
void set_path(char **paths, int count, shell_state_t *state);

#endif 