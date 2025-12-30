#include "../include/shell.h"

/* Check if command is built-in */
int is_builtin(char *cmd)
{
    if (cmd == NULL) return 0;
    return (strcmp(cmd, "exit") == 0 || strcmp(cmd, "cd") == 0 ||
        strcmp(cmd, "env") == 0 || strcmp(cmd, "setenv") == 0 ||
        strcmp(cmd, "unsetenv") == 0 || strcmp(cmd, "alias") == 0 ||
        strcmp(cmd, "path") == 0);
}

/* Setup redirection */
static int setup_redirection(command_t *cmd)
{
    if (cmd->output_file != NULL) {
        int fd = open(cmd->output_file, O_WRONLY | O_CREAT | O_TRUNC, 0644);
        if (fd < 0) {
            print_error();
            return -1;
        }
        
        if (dup2(fd, STDOUT_FILENO) < 0) {
            close(fd);
            print_error();
            return -1;
        }
        
        if (dup2(fd, STDERR_FILENO) < 0) {
            close(fd);
            print_error();
            return -1;
        }
        
        close(fd);
    }
    
    return 0;
}

/* Execute external command */
int execute_external(command_t *cmd, shell_state_t *state)
{
    pid_t pid;
    int status;
    
    /* Find command */
    char *cmd_path = find_command_in_path(cmd->args[0], state);
    if (cmd_path == NULL) {
        fprintf(stderr, "%s: command not found\n", cmd->args[0]);
        return 127;
    }
    
    pid = fork();
    if (pid < 0) {
        free(cmd_path);
        print_error();
        return 1;
    }
    
    if (pid == 0) {
        /* Child process */
        if (setup_redirection(cmd) != 0) {
            free(cmd_path);
            exit(1);
        }
        
        if (execv(cmd_path, cmd->args) == -1) {
            if (errno == EACCES) {
                fprintf(stderr, "%s: permission denied\n", cmd->args[0]);
                free(cmd_path);
                exit(126);
            }
            perror("execv");
            free(cmd_path);
            exit(1);
        }
    } else {
        /* Parent process */
        free(cmd_path);
        
        if (cmd->background) {
            /* Background process - don't wait */
            printf("[%d]\n", pid);
            state->last_exit_status = 0;
            return 0;
        } else {
            /* Wait for foreground process */
            waitpid(pid, &status, 0);
            
            if (WIFEXITED(status)) {
                state->last_exit_status = WEXITSTATUS(status);
                return state->last_exit_status;
            } else {
                state->last_exit_status = 1;
                return 1;
            }
        }
    }
    
    return 0;
}

/* Execute built-in command */
int execute_builtin(command_t *cmd, shell_state_t *state)
{
    int result;
    
    if (strcmp(cmd->args[0], "exit") == 0) {
        result = builtin_exit(cmd, state);
    } else if (strcmp(cmd->args[0], "cd") == 0) {
        result = builtin_cd(cmd, state);
    } else if (strcmp(cmd->args[0], "env") == 0) {
        result = builtin_env(cmd, state);
    } else if (strcmp(cmd->args[0], "setenv") == 0) {
        result = builtin_setenv(cmd, state);
    } else if (strcmp(cmd->args[0], "unsetenv") == 0) {
        result = builtin_unsetenv(cmd, state);
    } else if (strcmp(cmd->args[0], "alias") == 0) {
        result = builtin_alias(cmd, state);
    } else if (strcmp(cmd->args[0], "path") == 0) {
        result = builtin_path(cmd, state);
    } else {
        result = 1;
    }
    
    state->last_exit_status = result;
    return result;
}

/* Execute command chain */
/* Execute command chain */
static int execute_chain(command_t *cmd, shell_state_t *state)
{
    command_t *current = cmd;
    int result = 0;
    
    while (current != NULL && !state->exit_requested) {
        if (current->args[0] == NULL) {
            /* Empty command */
            result = 0;
        } else if (is_builtin(current->args[0])) {
            result = execute_builtin(current, state);
        } else {
            result = execute_external(current, state);
        }
        
        /* If current command is background, return immediately */
        if (current->background) {
            return result;  /* Don't wait for next commands */
        }
        
        /* Handle operators */
        switch (current->next_op) {
            case OP_SEQUENCE:
                /* Always execute next command */
                break;
                
            case OP_AND:
                /* Execute next only if current succeeded */
                if (result != 0) {
                    return result; /* Skip rest of chain */
                }
                break;
                
            case OP_OR:
                /* Execute next only if current failed */
                if (result == 0) {
                    return result; /* Skip rest of chain */
                }
                break;
                
            case OP_BACKGROUND:
            case OP_REDIRECT:
            case OP_NONE:
                /* Normal termination */
                return result;
        }
        
        current = current->next;
    }
    
    return result;
}
/* Main execution function */
int execute_command(command_t *cmd, shell_state_t *state)
{
    if (cmd == NULL || cmd->args[0] == NULL) {
        state->last_exit_status = 0;
        return 0;
    }
    
    return execute_chain(cmd, state);
}