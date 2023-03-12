//Определим типы byte (байт), word (слово), address (адрес байта или слова).
#include <stdio.h>

#define MEMSIZE (64*1024)   // размер памяти 64 килобайта

byte mem[MEMSIZE];          // объявляем массив mem - "память" компьютера PDP-11 как глобальную переменную


typedef char byte;

typedef short word;

typedef word address;

void b_write (adress adr, byte val); // пишем значение (байт) val по адресу adr

byte b_read (adress adr);            // читаем байт по адресу adr и возвращаем его

void w_write (adress adr, word val); // пишем значение (слово) val по адресу adr

word w_read (adress adr);            // читаем слово по адресу adr и возвращаем его

void size_of_data()
{
	printf("byte = %zd\n", sizeof(byte));
	printf("word = %zd\n", sizeof(word));
	printf("address = %zd\n", sizeof(address));
}

int main()
{
	size_of_data();
	return 0;
}