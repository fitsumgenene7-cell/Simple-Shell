#ifndef SHELL_H
#define SHELL_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <errno.h>
#include <signal.h>
#include <ctype.h>

/* Constants */
#define MAX_INPUT 4096
#define MAX_ARGS 128
#define MAX_PATH_LEN 4096
#define MAX_ALIASES 100
#define PROMPT "$ "
#define ERROR_MSG "An error has occurred\n"

/* Shell modes */
typedef enum {
    MODE_INTERACTIVE,
    MODE_BATCH,
    MODE_PIPE
} shell_mode_t;

/* Operator types */
typedef enum {
    OP_NONE,
    OP_SEQUENCE,    /* ; */
    OP_AND,         /* && */
    OP_OR,          /* || */
    OP_BACKGROUND,  /* & */
    OP_REDIRECT     /* > */
} operator_t;

/* Command structure */
typedef struct command_s {
    char **args;                /* Command arguments */
    char *output_file;          /* For output redirection (>) */
    int background;             /* Run in background? */
    operator_t next_op;         /* Operator to next command */
    struct command_s *next;     /* Next command in chain */
} command_t;

/* Shell state structure */
typedef struct shell_state_s {
    shell_mode_t mode;
    int exit_requested;
    int exit_status;
    int last_exit_status;
    
    /* Directories */
    char *cwd;
    char *home;
    char *oldpwd;
    
    /* Process ID */
    pid_t shell_pid;
    
    /* Path handling */
    char **path_dirs;
    int path_count;
    
    /* Batch mode */
    char *batch_file;
    FILE *batch_fp;
} shell_state_t;

/* Function prototypes */

/* shell.c functions */
void init_shell(shell_state_t *state, int argc, char **argv);
void run_shell(shell_state_t *state);
void cleanup_shell(shell_state_t *state);
char *read_input(shell_state_t *state);

/* parser.c functions */
command_t *parse_command(char *input, shell_state_t *state);
void free_command(command_t *cmd);

/* execute.c functions */
int execute_command(command_t *cmd, shell_state_t *state);
int execute_builtin(command_t *cmd, shell_state_t *state);
int execute_external(command_t *cmd, shell_state_t *state);
int is_builtin(char *cmd);

/* Built-in commands */
int builtin_exit(command_t *cmd, shell_state_t *state);
int builtin_cd(command_t *cmd, shell_state_t *state);
int builtin_env(command_t *cmd, shell_state_t *state);
int builtin_setenv(command_t *cmd, shell_state_t *state);
int builtin_unsetenv(command_t *cmd, shell_state_t *state);
int builtin_alias(command_t *cmd, shell_state_t *state);
int builtin_path(command_t *cmd, shell_state_t *state);

/* Utility functions */
void print_error(void);
char **split_string(char *str, const char *delim, int *count);
void free_string_array(char **array);
char *trim_whitespace(char *str);
char *my_strdup(const char *s);

/* Signal handling */
void setup_signals(void);

/* PATH handling */
void init_path(shell_state_t *state);
char *find_command_in_path(char *cmd, shell_state_t *state);

#endif /* SHELL_H */
