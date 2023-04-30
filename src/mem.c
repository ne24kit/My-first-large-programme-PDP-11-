#include "mem.h"
#include <stdlib.h>

static unsigned char * mem;		  // объявляем массив mem - "память" компьютера PDP-11 как глобальную переменную

extern int size_mem_mal;

word reg[REGSIZE];   // массив регистров R0..R7

word reg_os;
word reg_od;

void create_mem()
{
	mem = malloc(sizeof(unsigned char)*size_mem_mal*1024);
}

void destroy_mem()
{
	free(mem);
}

void b_write (address adr, byte val)
{
	if(adr < 8) {
		reg[adr] = val;
		if ((val >> 7) & 1)
			reg[adr] |= 0xFF00;
		return;
	}
	if (adr == ostat) {
		reg_os = 0xFFFF;
		return;
	}
	if (adr == odata) {
		putchar(val);
		return;
	}
	
	if (adr >= size_mem_mal*1024) {
		Log(ERROR, "\nВыход за границы массива\n");
		destroy_mem();
		exit(0);
	}
	mem[adr] = val;
	
	
	
}

byte b_read (address adr)
{
	if(adr < 8)
		return reg[adr];
	if (adr == ostat)
		return 0xFF;
	if (adr == odata) 
		return reg_od;
	
	if (adr >= size_mem_mal*1024) {
		Log(ERROR, "\nВыход за границы массива\n");
		destroy_mem();
		exit(0);
	}
	
	return mem[adr];
	
}
void w_write (address adr, word val)
{
	if(adr < 8) {
		reg[adr] = val;
		return;
	}
	
	if(adr % 2 != 0)
	{
		Log(ERROR, "Попытка записать слово по нечетному адресу!\n");
		destroy_mem();
		exit(1);
	}
	
	if (adr == ostat) {
		reg_os = 0xFFFF;
		return;
	}
	if (adr == odata) {
		putchar(val);
		return;
	}
	if (adr >= size_mem_mal*1024) {
		fprintf(stderr, "adr = %o", adr);
		Log(ERROR, "\n1Выход за границы массива\n");
		destroy_mem();
		exit(1);
	}
	
	
	mem[adr] = (byte)val;
	mem[adr + 1] = (byte)(val >> 8);
	
}
word w_read (address adr)
{
	if (adr == ostat)
		return 0xFFFF;
	if (adr == odata) 
		return reg_od;
	if (adr >= size_mem_mal*1024) {
		Log(ERROR, "\nВыход за границы массива\n");
		destroy_mem();
		exit(0);
	}
	
	word w = mem[adr+1];
	w = w << 8;
	w |= mem[adr];
	return w & 0xFFFF;
}

void set_ostat()
{
	reg_os = 0xFFFF;
	reg_od = 0;
}

void mem_dump(address adr, int size)
{
	for(int i = 0; i < size; i += 2)
		printf("%06o: %06o %04x\n", adr + i, w_read(adr + i), w_read(adr + i));
}

void mem_clear()
{
	memset(mem, 0, sizeof(mem));
}
