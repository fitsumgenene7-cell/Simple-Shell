#include "../include/shell.h"

int main(int argc, char **argv)
{
    shell_state_t state;
    
    init_shell(&state, argc, argv);
    run_shell(&state);
    cleanup_shell(&state);
    
    return state.exit_status;
}