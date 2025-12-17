/* include/signals.h - Signal handling prototypes */

#ifndef SIGNALS_H
#define SIGNALS_H

#include "shell.h"

/* Setup signal handlers */
void setup_signals(void);

/* SIGINT handler */
void handle_sigint(int sig);

#endif /* SIGNALS_H */