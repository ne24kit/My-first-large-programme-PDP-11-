#pragma once

#include <stdio.h>

#include <stdlib.h>

#include <errno.h>

#include <string.h>

#include "log.h"

#define MEMSIZE (64*1024)   // размер памяти 64 килобайта
#define pc reg[7]           // к rep[7] можем обращаться как pc
#define REGSIZE 8

typedef unsigned char byte;

typedef unsigned short word;

typedef word address;

void b_write (address adr, byte val); // пишем значение (байт) val по адресу adr

byte b_read (address adr);			// читаем байт по адресу adr и возвращаем его

void w_write (address adr, word val); // пишем значение (слово) val по адресу adr

word w_read (address adr);			// читаем слово по адресу adr и возвращаем его

void how_to_use_keys(const char * progname);

void load_data();

void mem_dump(address adr, int size);

void load_file(const char * filename);
