#include "header.h"

int level;

int set_log_level(int new_level)
{
	
	int old_level = level;
	level = new_level;
	
	return old_level;
}

void log(int log_level, const char * format, ...)
{
	if (level >= log_level) {
		va_list ap;
		va_start(ap, format);
		vfprintf(stdout, format, ap);
		va_end(ap);
	}
}

void load_file(const char * filename)
{	
	FILE * fin  = fopen(filename, "r");   // открыть файл data.txt на чтение - поток fin
	if (fin == NULL) {
		perror("ERROR");
		log(ERROR, "FILE:  %s\n", filename);
		exit(errno);
	}
	
	address adr;
	address size;
	byte val;
	while(fscanf(fin, "%04hx %04hx", &adr, &size) != EOF){
		for(address i = 0; i < size; i++){
			fscanf(fin, "%04hhx", &val);
			b_write(adr+i, val);
		}
	}
	fclose(fin);	
}

void mem_dump(address adr, int size)
{
	for(int i = 0; i < size; i += 2)
		printf("%06o: %06o %04x\n", adr + i, w_read(adr + i), w_read(adr + i));
}
void size_of_data()
{
	printf("byte = %zd\n", sizeof(byte));
	printf("word = %zd\n", sizeof(word));
	printf("address = %zd\n", sizeof(address));
}

void b_write (address adr, byte val)
{
	mem[adr] = val;
}

byte b_read (address adr)
{
	return mem[adr];
}
void w_write (address adr, word val)
{
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

void test_mem()
{
	address a;
	byte b0, b1, bres;
	word w, wres;


	// пишем байт, читаем байт
	log(DEBUG, "Пишем и читаем байт по четному адресу\n");
	a = 0;
	b0 = 0x12;
	b_write(a, b0);
	bres = b_read(a);
	log(DEBUG, "a=%06o b0=%hhx bres=%hhx\n", a, b0, bres);
	assert(b0 == bres);
	
	log(DEBUG, "Пишем и читаем байт по НЕчетному адресу\n");
	a = 1;		// другой адрес
	b0 = 0x34;
	b_write(a, b0);
	bres = b_read(a);
	log(DEBUG, "a=%06o b0=%hhx bres=%hhx\n", a, b0, bres);
	assert(b0 == bres);

	// пишем слово, читаем слово
	log(DEBUG, "Пишем и читаем слово\n");
	a = 2;		// другой адрес
	w = 0x3456;
	w_write(a, w);
	wres = w_read(a);
	log(DEBUG, "a=%06o w=%04x wres=%04x\n", a, w, wres);
	assert(w == wres);


	// пишем 2 байта, читаем 1 слово
	log(DEBUG, "Пишем 2 байта, читаем слово\n");
	a = 4;		// другой адрес
	w = 0xa1b2;
	// little-endian, младшие разряды по меньшему адресу
	b0 = 0xb2;
	b1 = 0xa1;
	b_write(a, b0);
	b_write(a+1, b1);
	wres = w_read(a);
	log(DEBUG, "a=%06o b1=%02hhx b0=%02hhx wres=%04x\n", a, b1, b0, wres);
	assert(w == wres);
	
	log(DEBUG, "Тесты пройдены успешно!\n");
}

void how_to_use_keys(const char * progname)
{
	printf("to compile use: %s [-t] <filename>\n	<filename> - <input data>\n", progname);
}

int main(int argc, char * argv[])
{	
	set_log_level(DEBUG);
	//test_mem();

	if(argc == 1 || argc == 2) {
		how_to_use_keys(argv[0]);
		exit(1);
	}
	
	const char * filename = argv[argc-1];
	
	char trace = 0;
	
	if (argc == 3 && (0 == strcmp("-t", argv[1])))
		trace = 1;
	
	load_file(argv[2]);
	
	mem_dump(0x0200,  0x000c);
	
	return 0;
}
