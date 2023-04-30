#pragma once

#include <stdio.h>

#include "log.h"

#include "mem.h"

#define NO_PARAMS 0
#define HAS_DD 1
#define HAS_SS 2
#define HAS_NN 4
#define HAS_R  8
#define HAS_XX 16

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

signed char get_xx(word w);

Arg get_r(word w);

void do_jsr();

void do_rts();

void do_clr();

void do_br();

void do_bcc();

void do_bcs();

void do_beq();

void do_bne();

void do_bmi();

void do_bpl();

void do_tst();

void set_NZ(word w);

void set_C(unsigned int w);

void is_byte_cmd(word w);

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

void do_movb();

void do_sob();

void do_inc();

void do_nothing();
