#include "../include/shell.h"
#include <limits.h>

/* Helper function to convert pid_t to string */
static char *pid_to_string(pid_t pid)
{
    static char buffer[20];
    snprintf(buffer, sizeof(buffer), "%d", (int)pid);
    return buffer;
}

/* Expand a single variable */
static char *expand_single_var(const char *var_name, shell_state_t *state)
{
    if (strcmp(var_name, "?") == 0) {
        /* Exit status */
        char *result = malloc(12); /* Max 10 digits for int + null */
        if (result) snprintf(result, 12, "%d", state->last_exit_status);
        return result;
    }
    else if (strcmp(var_name, "$") == 0) {
        /* Shell PID */
        return my_strdup(pid_to_string(state->shell_pid));
    }
    else if (strcmp(var_name, "HOME") == 0) {
        /* Home directory */
        return my_strdup(state->home);
    }
    else if (strcmp(var_name, "PWD") == 0) {
        /* Current directory */
        return my_strdup(state->cwd);
    }
    else if (strcmp(var_name, "OLDPWD") == 0) {
        /* Old directory */
        return my_strdup(state->oldpwd);
    }
    else {
        /* Environment variable */
        char *env_value = getenv(var_name);
        if (env_value) {
            return my_strdup(env_value);
        }
    }
    
    /* Variable not found - return empty string */
    return my_strdup("");
}

/* Main expansion function */
char *expand_variables(char *arg, shell_state_t *state)
{
    if (!arg) return NULL;
    
    /* Check if argument contains any $ */
    if (strchr(arg, '$') == NULL) {
        return my_strdup(arg);
    }
    
    /* Calculate maximum possible expanded size */
    size_t max_expansion = strlen(arg) * 2 + PATH_MAX + 20;
    char *result = malloc(max_expansion);
    if (!result) {
        print_error();
        return NULL;
    }
    
    char *src = arg;
    char *dest = result;
    
    while (*src) {
        if (src[0] == '$' && (isalnum((unsigned char)src[1]) || 
                              src[1] == '?' || src[1] == '$' || src[1] == '_')) {
            /* Found a variable */
            src++; /* Skip $ */
            
            char var_name[256];
            int i = 0;
            
            /* Extract variable name */
            if (*src == '?') {
                var_name[0] = '?';
                var_name[1] = '\0';
                src++;
            }
            else if (*src == '$') {
                var_name[0] = '$';
                var_name[1] = '\0';
                src++;
            }
            else {
                /* Alphanumeric variable name */
                while (isalnum((unsigned char)*src) || *src == '_') {
                    if (i < 255) {
                        var_name[i++] = *src;
                    }
                    src++;
                }
                var_name[i] = '\0';
            }
            
            /* Expand the variable */
            char *expanded = expand_single_var(var_name, state);
            if (expanded) {
                strcpy(dest, expanded);
                dest += strlen(expanded);
                free(expanded);
            }
        }
        else if (src[0] == '$' && src[1] == '{') {
            /* Handle ${VAR} syntax - skip for now or implement similarly */
            /* Skip to next character for now */
            *dest++ = *src++;
        }
        else {
            /* Regular character */
            *dest++ = *src++;
        }
    }
    
    *dest = '\0';
    
    /* Trim the result to actual size */
    char *final_result = my_strdup(result);
    free(result);
    
    return final_result;
}