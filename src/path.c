#include "../include/shell.h"

/* Initialize default path (/bin) */
void init_path(shell_state_t *state)
{
    state->path_count = 1;
    state->path_dirs = malloc(sizeof(char *));
    if (state->path_dirs == NULL) {
        return;
    }
    state->path_dirs[0] = my_strdup("/bin");
}

/* Find command in PATH */
char *find_command_in_path(char *cmd, shell_state_t *state)
{
    /* Check if command contains a slash (absolute or relative path) */
    if (strchr(cmd, '/') != NULL) {
        /* Absolute or relative path */
        if (access(cmd, F_OK) == 0) {
            return my_strdup(cmd);
        }
        return NULL;
    }
    
    /* Search in PATH directories */
    for (int i = 0; i < state->path_count; i++) {
        if (state->path_dirs[i] == NULL) {
            continue;
        }
        
        /* Build full path: directory + "/" + command */
        size_t dir_len = strlen(state->path_dirs[i]);
        size_t cmd_len = strlen(cmd);
        size_t total_len = dir_len + 1 + cmd_len + 1; /* +1 for "/", +1 for null */
        
        char *full_path = malloc(total_len);
        if (full_path == NULL) {
            return NULL;
        }
        
        /* Construct full path */
        snprintf(full_path, total_len, "%s/%s", state->path_dirs[i], cmd);
        
        /* Check if file exists */
        if (access(full_path, F_OK) == 0) {
            return full_path;
        }
        
        free(full_path);
    }
    
    /* Command not found in any PATH directory */
    return NULL;
}

/* Set new PATH (for path built-in command) */
void set_path(char **paths, int count, shell_state_t *state)
{
    /* Free old paths */
    if (state->path_dirs != NULL) {
        for (int i = 0; i < state->path_count; i++) {
            free(state->path_dirs[i]);
        }
        free(state->path_dirs);
    }
    
    /* Set new paths */
    state->path_count = count;
    if (count > 0) {
        state->path_dirs = malloc(sizeof(char *) * count);
        for (int i = 0; i < count; i++) {
            state->path_dirs[i] = my_strdup(paths[i]);
        }
    } else {
        state->path_dirs = NULL;
    }
}