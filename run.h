#pragma once

#include <stdio.h>

#include "log.h"

#include "mem.h"

typedef struct {
    word mask;
    word opcode;
    char * name;
    void (*do_command)(void);
} Command;

void run(void);

void do_halt();

void do_add();

void do_mov();

void do_sob();

void do_inc();

void do_nothing();