
#ifndef SIGNALS_H
#define SIGNALS_H

#include "shell.h"

void setup_signals(void);

void handle_sigint(int sig);

#endif 