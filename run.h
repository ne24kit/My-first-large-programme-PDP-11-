#pragma once

#include <stdio.h>

#include "log.h"

#include "mem.h"

#define NO_PARAMS 0
#define HAS_DD 1
#define HAS_SS 2
#define HAS_NN 4
#define HAS_R  8

typedef struct {
    word mask;
    word opcode;
    char * name;
    void (*do_command)(void);
	char params;
} Command;

typedef struct {
    word val;     // значение (что)
    address adr;    // адрес (куда)
} Arg;

Arg get_mr(word w);

Arg get_rn(word w);

void registers_on_null();

Command parse_cmd(word w);

word read_cmd();

void reg_dump();

void run();

void do_halt();

void do_add();

void do_mov();

void do_sob();

void do_inc();

void do_nothing();
