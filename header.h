#pragma once

//Определим типы byte (байт), word (слово), address (адрес байта или слова).
#include <stdio.h>
#include <assert.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <stdarg.h>

#define MEMSIZE (64*1024)   // размер памяти 64 килобайта

enum LOGGING_LEVELS {
	ERROR,
	INFO,
	TRACE,
	DEBUG
};

typedef unsigned char byte;

typedef unsigned short word;

typedef word address;

byte mem[MEMSIZE];		  // объявляем массив mem - "память" компьютера PDP-11 как глобальную переменную

void b_write (address adr, byte val); // пишем значение (байт) val по адресу adr

byte b_read (address adr);			// читаем байт по адресу adr и возвращаем его

void w_write (address adr, word val); // пишем значение (слово) val по адресу adr

word w_read (address adr);			// читаем слово по адресу adr и возвращаем его

void test_mem();

void size_of_data();

void load_data();

void mem_dump(address adr, int size);

void load_file(const char * filename);

void how_to_use_keys(const char * progname);

void log(int level, const char * format, ...);

int set_log_level(int level);
