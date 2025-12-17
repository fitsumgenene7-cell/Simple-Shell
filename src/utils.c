#include "../include/shell.h"

/* Custom strdup for portability */
char *my_strdup(const char *s)
{
    if (s == NULL) return NULL;
    size_t len = strlen(s) + 1;
    char *dup = malloc(len);
    if (dup != NULL) {
        memcpy(dup, s, len);
    }
    return dup;
}

/* Split string into tokens */
char **split_string(char *str, const char *delim, int *count)
{
    char *token;
    char **tokens = NULL;
    int capacity = 10;
    int size = 0;
    
    tokens = malloc(capacity * sizeof(char *));
    if (tokens == NULL) return NULL;
    
    char *str_copy = my_strdup(str);
    if (str_copy == NULL) {
        free(tokens);
        return NULL;
    }
    
    token = strtok(str_copy, delim);
    while (token != NULL) {
        if (size >= capacity) {
            capacity *= 2;
            char **new_tokens = realloc(tokens, capacity * sizeof(char *));
            if (new_tokens == NULL) {
                free(str_copy);
                free_string_array(tokens);
                return NULL;
            }
            tokens = new_tokens;
        }
        
        tokens[size] = my_strdup(token);
        if (tokens[size] == NULL) {
            free(str_copy);
            free_string_array(tokens);
            return NULL;
        }
        size++;
        
        token = strtok(NULL, delim);
    }
    
    if (size >= capacity) {
        char **new_tokens = realloc(tokens, (size + 1) * sizeof(char *));
        if (new_tokens == NULL) {
            free(str_copy);
            free_string_array(tokens);
            return NULL;
        }
        tokens = new_tokens;
    }
    tokens[size] = NULL;
    
    if (count != NULL) {
        *count = size;
    }
    
    free(str_copy);
    return tokens;
}

/* Free array of strings */
void free_string_array(char **array)
{
    if (array == NULL) return;
    
    for (int i = 0; array[i] != NULL; i++) {
        free(array[i]);
    }
    free(array);
}

/* Trim whitespace from both ends */
char *trim_whitespace(char *str)
{
    char *end;
    
    while (isspace((unsigned char)*str)) str++;
    
    if (*str == 0) return str;
    
    end = str + strlen(str) - 1;
    while (end > str && isspace((unsigned char)*end)) end--;
    
    *(end + 1) = '\0';
    return str;
}
