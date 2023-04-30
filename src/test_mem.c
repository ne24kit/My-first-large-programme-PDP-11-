#include "test_mem.h"

void test_mem()
{
	address a;
	byte b0, b1, bres;
	word w, wres;


	// пишем байт, читаем байт
	Log(DEBUG, "Пишем и читаем байт по четному адресу\n");
	a = 10;
	b0 = 0x12;
	b_write(a, b0);
	bres = b_read(a);
	Log(DEBUG, "a=%06o b0=%hhx bres=%hhx\n", a, b0, bres);
	assert(b0 == bres);
	
	Log(DEBUG, "Пишем и читаем байт по НЕчетному адресу\n");
	a = 11;		// другой адрес
	b0 = 0x34;
	b_write(a, b0);
	bres = b_read(a);
	Log(DEBUG, "a=%06o b0=%hhx bres=%hhx\n", a, b0, bres);
	assert(b0 == bres);

	// пишем слово, читаем слово
	Log(DEBUG, "Пишем и читаем слово\n");
	a = 12;		// другой адрес
	w = 0x3456;
	w_write(a, w);
	wres = w_read(a);
	Log(DEBUG, "a=%06o w=%04x wres=%04x\n", a, w, wres);
	assert(w == wres);


	// пишем 2 байта, читаем 1 слово
	Log(DEBUG, "Пишем 2 байта, читаем слово\n");
	a = 14;		// другой адрес
	w = 0xa1b2;
	// little-endian, младшие разряды по меньшему адресу
	b0 = 0xb2;
	b1 = 0xa1;
	b_write(a, b0);
	b_write(a+1, b1);
	wres = w_read(a);
	Log(DEBUG, "a=%06o b1=%02hhx b0=%02hhx wres=%04x\n", a, b1, b0, wres);
	assert(w == wres);
	
	//пишем 1 слово, читаем 2 байта
	Log(DEBUG, "Пишем 2 байта, читаем слово\n");
	a = 16;		// другой адрес
	w = 0xa1b2;
	b0 = 0xb2;
	b1 = 0xa1;
	// little-endian, младшие разряды по меньшему адресу
	w_write(a, w);
	byte b0_res = b_read(a);
	byte b1_res = b_read(a+1);

	Log(DEBUG, "a=%06o b1=%02hhx b0=%02hhx b1_res=%02hhx b0_res=%02hhx\n ", a, b1, b0, b1_res, b0_res);
	assert(b0 == b0_res && b1 == b1_res);
	
	Log(DEBUG, "Тесты на работу с памятью пройдены успешно!\n");
}

void size_of_data()
{
	printf("byte = %zd\n", sizeof(byte));
	printf("word = %zd\n", sizeof(word));
	printf("address = %zd\n", sizeof(address));
}
