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

/* Execute built-in command */
int execute_builtin(command_t *cmd, shell_state_t *state)
{
    if (strcmp(cmd->args[0], "exit") == 0) {
        state->exit_requested = 1;
        if (cmd->args[1] != NULL) {
            state->exit_status = atoi(cmd->args[1]);
        }
        return state->exit_status;
    } else if (strcmp(cmd->args[0], "cd") == 0) {
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
    } else if (strcmp(cmd->args[0], "env") == 0) {
        /* For now, print a placeholder */
        printf("env: Prints environment variables\n");
        return 0;
    } else if (strcmp(cmd->args[0], "setenv") == 0) {
        printf("setenv: Sets environment variable\n");
        return 0;
    } else if (strcmp(cmd->args[0], "unsetenv") == 0) {
        printf("unsetenv: Unsets environment variable\n");
        return 0;
    } else if (strcmp(cmd->args[0], "alias") == 0) {
        printf("alias: Manages aliases\n");
        return 0;
    } else if (strcmp(cmd->args[0], "path") == 0) {
        printf("path: Sets PATH\n");
        return 0;
    }
    
    return 1;
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
            /* Background process */
            printf("[%d]\n", pid);
            return 0;
        } else {
            /* Wait for foreground process */
            waitpid(pid, &status, 0);
            
            if (WIFEXITED(status)) {
                return WEXITSTATUS(status);
            } else {
                return 1;
            }
        }
    }
    
    return 0;
}

/* Main execution function */
int execute_command(command_t *cmd, shell_state_t *state)
{
    if (cmd == NULL || cmd->args[0] == NULL) {
        return 0;
    }
    
    if (is_builtin(cmd->args[0])) {
        return execute_builtin(cmd, state);
    } else {
        return execute_external(cmd, state);
    }
}
