//Определим типы byte (байт), word (слово), address (адрес байта или слова).
#include <stdio.h>

#define MEMSIZE (64*1024)   // размер памяти 64 килобайта

byte mem[MEMSIZE];          // объявляем массив mem - "память" компьютера PDP-11 как глобальную переменную


typedef char byte;

typedef short word;

typedef word address;

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