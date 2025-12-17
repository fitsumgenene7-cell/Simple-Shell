/* src/signals.c - Signal handling (Ctrl+C, Ctrl+D) */

#include "../include/shell.h"
#include <signal.h>

/* SIGINT handler (Ctrl+C) */
void handle_sigint(int sig)
{
    (void)sig; /* Unused parameter */
    
    /* Do nothing - just ignore the signal */
    printf("\n"); /* Print newline for clean prompt */
    printf(PROMPT);
    fflush(stdout);
}

/* Setup signal handlers */
void setup_signals(void)
{
    struct sigaction sa;
    
    /* Setup SIGINT (Ctrl+C) handler */
    sa.sa_handler = handle_sigint;
    sa.sa_flags = 0;
    sigemptyset(&sa.sa_mask);
    
    if (sigaction(SIGINT, &sa, NULL) == -1) {
        perror("sigaction");
    }
    
    /* Ignore SIGQUIT (Ctrl+\) */
    signal(SIGQUIT, SIG_IGN);
}