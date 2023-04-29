#pragma once

#include <stdio.h>

#include <stdlib.h>

#include <errno.h>

#include <string.h>

#include "log.h"

#define MEMSIZE (64*1024)   // размер памяти 64 килобайта
#define pc reg[7]   // к rep[7] можем обращаться как pc
#define sp reg[6]   // к rep[6] можем обращаться как sp
#define REGSIZE 8
#define ostat 0177564
#define odata 0177566

typedef unsigned char byte;

typedef unsigned short word;

typedef word address;

void set_ostat(); 

void create_mem();

void destroy_mem();

void b_write (address adr, byte val); // пишем значение (байт) val по адресу adr

byte b_read (address adr);			// читаем байт по адресу adr и возвращаем его

void w_write (address adr, word val); // пишем значение (слово) val по адресу adr

word w_read (address adr);			// читаем слово по адресу adr и возвращаем его

void load_data();

void mem_dump(address adr, int size);

void mem_clear();
