#include "../include/shell.h"
#include <stdbool.h>

static bool is_operator_char(char c)
{
    return (c == ';' || c == '&' || c == '|' || c == '>' || c == '#');
}

static char *skip_whitespace(char *str)
{
    while (*str && isspace((unsigned char)*str)) {
        str++;
    }
    return str;
}


/* Get operator type from string */
static operator_t get_operator(char *str)
{
    if (str[0] == ';') return OP_SEQUENCE;
    if (str[0] == '&' && str[1] == '&') return OP_AND;
    if (str[0] == '|' && str[1] == '|') return OP_OR;
    if (str[0] == '&') return OP_BACKGROUND;
    if (str[0] == '>') return OP_REDIRECT;
    return OP_NONE;
}

static command_t *parse_single_command(char **input_ptr, shell_state_t *state)
{
    (void)state; /* Unused for now */
    
    char *pos = *input_ptr;
    command_t *cmd = malloc(sizeof(command_t));
    if (!cmd) return NULL;
    
    memset(cmd, 0, sizeof(command_t));
    cmd->args = malloc(MAX_ARGS * sizeof(char *));
    if (!cmd->args) {
        free(cmd);
        return NULL;
    }
    
    int arg_count = 0;
    
    while (*pos && *pos != '#') {
        pos = skip_whitespace(pos);
        if (!*pos || *pos == '#') break;
        
        /* Check for operators that end or modify this command */
        operator_t op = get_operator(pos);
        
        if (op == OP_REDIRECT) {
            /* Handle output redirection */
            pos++; /* Skip '>' */
            pos = skip_whitespace(pos);
            
            char *start = pos;
            while (*pos && !isspace((unsigned char)*pos) && !is_operator_char(*pos)) {
                pos++;
            }
            
            if (pos > start) {
                size_t len = pos - start;
                cmd->output_file = malloc(len + 1);
                strncpy(cmd->output_file, start, len);
                cmd->output_file[len] = '\0';
            } else {
                print_error();
                free_command(cmd);
                return NULL;
            }
            continue;
        }
        else if (op == OP_BACKGROUND) {
            /* Check if it's truly a background operator (not part of &&) */
            char next_char = *(pos + 1);
            if (isspace((unsigned char)next_char) || !next_char || is_operator_char(next_char)) {
                cmd->background = 1;
                pos++;
                continue;
            }
            /* If not, fall through to treat as part of argument */
        }
        else if (op != OP_NONE) {
            /* Other operators end this command */
            break;
        }
        
        /* Extract regular argument */
        char *start = pos;
        while (*pos && !isspace((unsigned char)*pos) && !is_operator_char(*pos)) {
            pos++;
        }
        
        if (pos > start) {
            size_t len = pos - start;
            if (arg_count >= MAX_ARGS - 1) {
                free_command(cmd);
                print_error();
                return NULL;
            }
            
            cmd->args[arg_count] = malloc(len + 1);
            strncpy(cmd->args[arg_count], start, len);
            cmd->args[arg_count][len] = '\0';
            arg_count++;
        }
    }
    
    cmd->args[arg_count] = NULL;
    *input_ptr = pos;
    
    return cmd;
}

static operator_t find_next_operator(char *str, int *op_len)
{
    char *pos = skip_whitespace(str);
    
    if (!*pos) return OP_NONE;
    
    operator_t op = get_operator(pos);
    
    switch (op) {
        case OP_AND:
        case OP_OR:
            *op_len = 2;
            break;
        case OP_SEQUENCE:
        case OP_BACKGROUND:
        case OP_REDIRECT:
            *op_len = 1;
            break;
        default:
            *op_len = 0;
            break;
    }
    
    return op;
}

/* Main parsing function */
command_t *parse_command(char *input, shell_state_t *state)
{
    command_t *head = NULL;
    command_t *tail = NULL;
    char *pos = input;
    
    while (*pos && *pos != '#') {
        pos = skip_whitespace(pos);
        if (!*pos || *pos == '#') break;
        
        command_t *cmd = parse_single_command(&pos, state);
        if (!cmd) continue;
        
        /* Find operator to next command */
        int op_len;
        operator_t next_op = find_next_operator(pos, &op_len);
        cmd->next_op = next_op;
        
        /* Skip the operator for next iteration */
        if (next_op != OP_NONE && op_len > 0) {
            pos += op_len;
        }
        
        /* Add to linked list */
        if (!head) {
            head = cmd;
            tail = cmd;
        } else {
            tail->next = cmd;
            tail = cmd;
        }
    }
    
    return head;
}

void free_command(command_t *cmd)
{
    if (!cmd) return;
    
    /* Free arguments */
    if (cmd->args) {
        for (int i = 0; cmd->args[i]; i++) {
            free(cmd->args[i]);
        }
        free(cmd->args);
    }
    
    /* Free output file */
    if (cmd->output_file) {
        free(cmd->output_file);
    }
    
    /* Free the command itself */
    free(cmd);
}
