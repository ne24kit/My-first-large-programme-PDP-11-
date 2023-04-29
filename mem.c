#include "mem.h"

static byte mem[MEMSIZE];		  // объявляем массив mem - "память" компьютера PDP-11 как глобальную переменную

word reg[REGSIZE];               // массив регистров R0..R7

void b_write (address adr, byte val)
{
	if(adr < 8) {
		reg[adr] = val;
		if ((val >> 7) & 1)
			reg[adr] |= 0xFF00;
		return;
	}
	if (adr == odata)
		putchar(val);
	mem[adr] = val;
	
}

byte b_read (address adr)
{
	if(adr < 8)
		return reg[adr];
	return mem[adr];
}
void w_write (address adr, word val)
{
	if(adr < 8) {
		reg[adr] = val;
		return;
	}
	if (adr == odata) {
		putchar(val);
	}
	
	mem[adr] = (byte)val;
	mem[adr + 1] = (byte)(val >> 8);
	
}
word w_read (address a)
{
	word w = mem[a+1];
	w = w << 8;
	w |= mem[a];
	return w & 0xFFFF;
}

void set_ostat()
{
	w_write(ostat, 0xFFFF); 
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
