#include "../include/shell.h"

/* External environment */
extern char **environ;

/* Initialize shell state */
void init_shell(shell_state_t *state, int argc, char **argv)
{
    /* Clear the structure */
    memset(state, 0, sizeof(shell_state_t));
    
    /* Determine shell mode */
    if (argc == 1) {
        /* No arguments - check if interactive */
        if (isatty(STDIN_FILENO)) {
            state->mode = MODE_INTERACTIVE;
        } else {
            state->mode = MODE_PIPE;
        }
    } else if (argc == 2) {
        /* One argument - batch mode */
        state->mode = MODE_BATCH;
        state->batch_file = my_strdup(argv[1]);
        if (state->batch_file == NULL) {
            print_error();
            exit(1);
        }
        
        /* Open batch file */
        state->batch_fp = fopen(state->batch_file, "r");
        if (state->batch_fp == NULL) {
            print_error();
            exit(1);
        }
    } else {
        /* Too many arguments */
        print_error();
        exit(1);
    }
    
    /* Initialize shell PID */
    state->shell_pid = getpid();
    
    /* Get current directory */
    state->cwd = getcwd(NULL, 0);
    if (state->cwd == NULL) {
        state->cwd = my_strdup(".");
    }
    
    /* Get home directory */
    state->home = getenv("HOME");
    if (state->home != NULL) {
        state->home = my_strdup(state->home);
    } else {
        state->home = my_strdup("/");
    }
    
    /* Initialize oldpwd */
    state->oldpwd = my_strdup(state->cwd);
    
    /* Initialize path */
    init_path(state);
    
    /* Set default exit status */
    state->exit_status = 0;
    state->last_exit_status = 0;
    state->exit_requested = 0;
}

/* Main shell loop */
void run_shell(shell_state_t *state)
{
    char *input = NULL;
    command_t *cmd = NULL;
    
    if (state->mode == MODE_INTERACTIVE) {
        setup_signals();
    }
    
    while (!state->exit_requested) {
        input = read_input(state);
        if (input == NULL) {
            if (state->mode == MODE_INTERACTIVE) {
                printf("\n");
            }
            break;
        }
        
        if (strlen(input) == 0 || input[0] == '#') {
            free(input);
            continue;
        }
        
        cmd = parse_command(input, state);
        if (cmd == NULL) {
            free(input);
            continue;
        }
        
        execute_command(cmd, state);
        free_command(cmd);
        free(input);
    }
}

/* Read input based on mode */
char *read_input(shell_state_t *state)
{
    char buffer[MAX_INPUT];
    char *input = NULL;
    
    /* Print prompt for interactive mode */
    if (state->mode == MODE_INTERACTIVE) {
        printf(PROMPT);
        fflush(stdout);
    }
    
    /* Read input */
    if (state->batch_fp != NULL) {
        /* Read from batch file */
        if (fgets(buffer, MAX_INPUT, state->batch_fp) == NULL) {
            return NULL;
        }
    } else {
        /* Read from stdin */
        if (fgets(buffer, MAX_INPUT, stdin) == NULL) {
            return NULL;
        }
    }
    
    /* Remove trailing newline */
    size_t len = strlen(buffer);
    if (len > 0 && buffer[len - 1] == '\n') {
        buffer[len - 1] = '\0';
    }
    
    /* Allocate and copy input */
    input = my_strdup(buffer);
    return input;
}

/* Cleanup shell resources */
void cleanup_shell(shell_state_t *state)
{
    /* Free strings */
    if (state->cwd != NULL) free(state->cwd);
    if (state->home != NULL) free(state->home);
    if (state->oldpwd != NULL) free(state->oldpwd);
    if (state->batch_file != NULL) free(state->batch_file);
    
    /* Free path directories */
    if (state->path_dirs != NULL) {
        for (int i = 0; i < state->path_count; i++) {
            free(state->path_dirs[i]);
        }
        free(state->path_dirs);
    }
    
    /* Close batch file */
    if (state->batch_fp != NULL) {
        fclose(state->batch_fp);
    }
}
