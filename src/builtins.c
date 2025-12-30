#include "../include/shell.h"

/* Built-in: exit */
int builtin_exit(command_t *cmd, shell_state_t *state)
{
    state->exit_requested = 1;
    if (cmd->args[1] != NULL) {
        state->exit_status = atoi(cmd->args[1]);
    }
    return state->exit_status;
}

/* Built-in: cd */
int builtin_cd(command_t *cmd, shell_state_t *state)
{
    char *target = cmd->args[1];
    if (target == NULL || strcmp(target, "~") == 0) {
        target = state->home;
    } else if (strcmp(target, "-") == 0) {
        target = state->oldpwd;
    }
    
    if (chdir(target) != 0) {
        print_error();
        return 1;
    }
    
    free(state->oldpwd);
    state->oldpwd = my_strdup(state->cwd);
    free(state->cwd);
    state->cwd = getcwd(NULL, 0);
    return 0;
}

/* Built-in: env */
int builtin_env(command_t *cmd, shell_state_t *state)
{
    (void)cmd; /* Unused parameter */
    (void)state; /* Unused parameter */
    
    /* Print environment variables */
    extern char **environ;
    for (char **env = environ; *env != NULL; env++) {
        printf("%s\n", *env);
    }
    return 0;
}

/* Built-in: setenv */
int builtin_setenv(command_t *cmd, shell_state_t *state)
{
    (void)state; /* Unused parameter */
    
    if (cmd->args[1] == NULL || cmd->args[2] == NULL) {
        print_error();
        return 1;
    }
    
    if (setenv(cmd->args[1], cmd->args[2], 1) != 0) {
        print_error();
        return 1;
    }
    
    return 0;
}

/* Built-in: unsetenv */
int builtin_unsetenv(command_t *cmd, shell_state_t *state)
{
    (void)state; /* Unused parameter */
    
    if (cmd->args[1] == NULL) {
        print_error();
        return 1;
    }
    
    if (unsetenv(cmd->args[1]) != 0) {
        print_error();
        return 1;
    }
    
    return 0;
}

/* Built-in: alias */
int builtin_alias(command_t *cmd, shell_state_t *state)
{
    (void)cmd; /* Unused parameter */
    (void)state; /* Unused parameter */
    
    /* Placeholder implementation */
    printf("alias: Manages aliases\n");
    return 0;
}

/* Built-in: path */
int builtin_path(command_t *cmd, shell_state_t *state)
{
    (void)cmd; /* Unused parameter */
    
    /* Free existing path directories */
    if (state->path_dirs != NULL) {
        for (int i = 0; i < state->path_count; i++) {
            free(state->path_dirs[i]);
        }
        free(state->path_dirs);
        state->path_dirs = NULL;
        state->path_count = 0;
    }
    
    /* If no arguments, clear PATH */
    if (cmd->args[1] == NULL) {
        if (setenv("PATH", "", 1) != 0) {
            print_error();
            return 1;
        }
        return 0;
    }
    
    /* Build new PATH from arguments */
    char new_path[MAX_PATH_LEN] = "";
    for (int i = 1; cmd->args[i] != NULL; i++) {
        if (i > 1) {
            strcat(new_path, ":");
        }
        strcat(new_path, cmd->args[i]);
    }
    
    /* Set the new PATH */
    if (setenv("PATH", new_path, 1) != 0) {
        print_error();
        return 1;
    }
    
    /* Reinitialize shell's path directories */
    init_path(state);
    
    return 0;
}