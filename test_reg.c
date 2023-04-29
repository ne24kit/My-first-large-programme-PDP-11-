#include "test_reg.h"

extern Arg ss, dd;

extern Command command[];

extern word reg[REGSIZE];

void tests_on_cmd()
{
	test_parse_mov();
	test_parse_add();
	test_parse_halt();
	test_mov();
	Log(ERROR, "Тесты на определение команд пройдены\n");
	test_mode0();
	test_mode1_from_mem_to_reg();
	test_mode1_from_reg_to_mem();
	test_mode1_from_mem_to_mem();
	test_mode2_from_mem_to_reg();
	test_mode2_from_reg_to_mem();
	test_mode2_from_mem_to_mem();
	test_mode3_from_mem_to_reg();
	test_mode3_from_reg_to_mem();
	test_mode3_from_mem_to_mem();
	test_mode4_from_mem_to_reg();
	test_mode4_from_reg_to_mem();
	test_mode4_from_mem_to_mem();
	test_mode5_from_mem_to_reg();
	test_mode5_from_reg_to_mem();
	test_mode5_from_mem_to_mem();
	test_mode6_from_mem_to_reg();
	test_mode6_from_reg_to_mem();
	test_mode6_from_mem_to_mem();
	Log(ERROR, "Тесты на работу мод 1 - 6 пройдены\n\n");
}

// тест, что мы правильно определяем команды mov, add, halt
void test_parse_mov()
{
    Command cmd = parse_cmd(0011654);
    assert(!strcmp(cmd.name, "mov"));
    Log(TRACE, "\nfile: %s, line: %u, function: %s is ... OK\n", __FILE__, __LINE__, __FUNCTION__ );
}

void test_parse_add()
{
    Command cmd = parse_cmd(0061654);
    assert(!strcmp(cmd.name, "add"));
    Log(TRACE, "\nfile: %s, line: %u, function: %s is ... OK\n", __FILE__, __LINE__, __FUNCTION__ );
}


void test_parse_halt()
{   
    Command cmd = parse_cmd(0);
    assert(!strcmp(cmd.name, "halt"));
    Log(TRACE, "\nfile: %s, line: %u, function: %s is ... OK\n", __FILE__, __LINE__, __FUNCTION__ );
}

// тест, что мы разобрали правильно аргументы ss и dd в mov R5, R3
void test_mode0()
{   
    reg[3] = 012;    // dd
    reg[5] = 034;    // ss
	//						   ssdd
    Command cmd = parse_cmd(0010503);
	
    assert(ss.val == 034);
    assert(ss.adr == 5);
    assert(dd.val == 012);
    assert(dd.adr == 3);
    Log(TRACE, "\nfile: %s, line: %u, function: %s is ... OK\n", __FILE__, __LINE__, __FUNCTION__ );
}
// тест, что mov и мода 0 работают верно в mov R5, R3
void test_mov()
{   
    reg[3] = 012;    // dd
    reg[5] = 034;    // ss
    Command cmd = parse_cmd(0010503);
    cmd.do_command();
    assert(reg[3] == 034);
    assert(reg[5] == 034);
    Log(TRACE, "\nfile: %s, line: %u, function: %s is ... OK\n", __FILE__, __LINE__, __FUNCTION__ );
}

void test_mode1_from_mem_to_reg() //mov (R5), R3
{
    // setup
    reg[5] = 0200;  // ss
    reg[3] = 12;    // dd
    w_write(0200, 34);

    Command cmd = parse_cmd(0011503);

    assert(ss.val == 34);
    assert(ss.adr == 0200);
    assert(dd.val == 12);
    assert(dd.adr == 3);

    cmd.do_command();

    assert(reg[3] == 34);
    // проверяем, что значение регистра не изменилось
    assert(reg[5] == 0200);

    Log(TRACE, "\nfile: %s, line: %u, function: %s is ... OK\n", __FILE__, __LINE__, __FUNCTION__ );
	
	mem_clear();                  //очистка памяти 
	registers_on_null();          //очистка регистров
}

void test_mode1_from_reg_to_mem () //mov R3, (R5)
{ 
	// setup
	reg[3] = 76;    //ss
	reg[5] = 0300;  //dd
	
	w_write(0300, 67);
	
	Command cmd = parse_cmd(0010315);
	
	assert(ss.val == 76);
	assert(ss.adr == 3);
	assert(dd.val == 67);
	assert(dd.adr == 0300);
	
	cmd.do_command();
	
	assert(reg[3] == 76);         //проверка, что НЕ изменилось значение в reg[3]
	assert(reg[5] == 0300);       //проверка, что значение (адресс) в reg[5] не изменилось
	assert(w_read(0300) == 76);   //проверка, что изменилось в reg[5]
	
	Log(TRACE, "\nfile: %s, line: %u, function: %s is ... OK\n", __FILE__, __LINE__, __FUNCTION__ );
	
	mem_clear();                  //очистка памяти 
	registers_on_null();          //очистка регистров
}

void test_mode1_from_mem_to_mem () //mov (R3), (R5)
{ 
	// setup
	reg[3] = 0300;    //ss
	reg[5] = 0500;  //dd
	
	w_write(0300, 67);
	w_write(0500, 76);
	
	Command cmd = parse_cmd(0011315);
	
	assert(ss.val == 67);
	assert(ss.adr == 0300);
	assert(dd.val == 76);
	assert(dd.adr == 0500);
	
	cmd.do_command();
	
	assert(reg[3] == 0300);       //проверка, что значение (адресс) в reg[3] не изменилось
	assert(reg[5] == 0500);       //проверка, что значение (адресс) в reg[5] не изменилось
	assert(w_read(0500) == 67);   //проверка, что изменилось в reg[5]
	
	Log(TRACE, "\nfile: %s, line: %u, function: %s is ... OK\n", __FILE__, __LINE__, __FUNCTION__ );
	
	mem_clear();                  //очистка памяти 
	registers_on_null();          //очистка регистров
}
//мода 2 увеличивает значение регистра на два
void test_mode2_from_mem_to_reg () //mov R3, (R5)+
{
	//setup
	reg[3] = 47;     //ss
	reg[5] = 0500;   //dd
	
	w_write(0500, 57);
	
	Command cmd = parse_cmd(0010325);
	
	assert(ss.val == 47);
	assert(ss.adr == 3);
	assert(dd.val == 57);
	assert(dd.adr == 0500);
	
	cmd.do_command();
	
	assert(reg[3] == 47);       //проверка, что значение (адресс) в reg[3] увеличилось на 2
	assert(w_read(0500) == 47); //проверка, что изменилось в памяти 
	assert(reg[5] == 0502);     //проверка, что значение (адресс) в reg[5] не изменился
	
	Log(TRACE, "\nfile: %s, line: %u, function: %s is ... OK\n", __FILE__, __LINE__, __FUNCTION__ );
	
	mem_clear();                  //очистка памяти 
	registers_on_null();          //очистка регистров
}

void test_mode2_from_reg_to_mem () //mov (R3)+, R5
{
	//setup
	reg[3] = 0300;     //ss
	reg[5] = 57;       //dd
	
	w_write(0300, 47);
	
	Command cmd = parse_cmd(0012305);
	
	assert(ss.val == 47);
	assert(ss.adr == 0300);
	assert(dd.val == 57);
	assert(dd.adr == 5);
	
	cmd.do_command();
	
	assert(reg[3] == 0302);         //проверка, что значение (адресс) в reg[3] увеличилось на 2
	assert(reg[5] == 47);           //проверка, что изменилось в памяти 
	assert(w_read(0300) == 47);     //проверка, что значение (адресс) в reg[5] не изменился
	
	Log(TRACE, "\nfile: %s, line: %u, function: %s is ... OK\n", __FILE__, __LINE__, __FUNCTION__ );
	
	mem_clear();                  //очистка памяти 
	registers_on_null();          //очистка регистров
}

void test_mode2_from_mem_to_mem () //mov (R3)+, (R5)+
{
	// setup
	reg[3] = 0300;    //ss
	reg[5] = 0500;  //dd
	
	w_write(0300, 67);
	w_write(0500, 76);
	
	Command cmd = parse_cmd(0012325);
	
	assert(ss.val == 67);
	assert(ss.adr == 0300);
	assert(dd.val == 76);
	assert(dd.adr == 0500);
	
	cmd.do_command();
	
	assert(reg[3] == 0302);   
	assert(reg[5] == 0502);   
	assert(w_read(0500) == 67);
	
	Log(TRACE, "\nfile: %s, line: %u, function: %s is ... OK\n", __FILE__, __LINE__, __FUNCTION__ );
	
	mem_clear();                  //очистка памяти 
	registers_on_null();          //очистка регистров
}

void test_mode3_from_mem_to_reg () //mov R3, @(R5)+
{
	
	//setup
	reg[3] = 47;     //ss
	reg[5] = 0500;   //dd
	
	w_write(0500, 0600);
	w_write(0600, 57);
	
	Command cmd = parse_cmd(0010335);
	
	assert(ss.val == 47);
	assert(ss.adr == 3);
	assert(dd.val == 57);
	assert(dd.adr == 0600);
	
	cmd.do_command();
	
	assert(reg[3] == 47);         //проверка, что значение в reg[3] не изменилось
	assert(w_read(0500) == 0600); //проверка, что не изменился в памяти 
	assert(w_read(0600) == 47);   //проверка, что в значение поменялось
	assert(reg[5] == 0502);       //проверка, что значение (адресс) в reg[5] не изменился
	
	Log(TRACE, "\nfile: %s, line: %u, function: %s is ... OK\n", __FILE__, __LINE__, __FUNCTION__ );
	
	mem_clear();                  //очистка памяти 
	registers_on_null();          //очистка регистров
}

void test_mode3_from_reg_to_mem () //mov @(R3)+, R5
{
	
	//setup
	reg[3] = 0300;     //ss
	reg[5] = 57;   //dd
	
	w_write(0300, 0400);
	w_write(0400, 47);
	
	Command cmd = parse_cmd(0013305);
	
	assert(ss.val == 47);
	assert(ss.adr == 0400);
	assert(dd.val == 57);
	assert(dd.adr == 5);
	
	cmd.do_command();
	
	assert(reg[3] == 0302);         //проверка, что значение (адресс) в reg[3] увеличилось на 2
	assert(w_read(0300) == 0400);   //проверка, что значение (адресс) в reg[5] не изменился
	assert(w_read(0400) == 47);     //проверка, что число 47 лежит по тому же адрессу
	assert(reg[5] == 47);           //проверка, что изменилось в памяти 
	
	Log(TRACE, "\nfile: %s, line: %u, function: %s is ... OK\n", __FILE__, __LINE__, __FUNCTION__ );
	
	mem_clear();                  //очистка памяти 
	registers_on_null();          //очистка регистров
}


void test_mode3_from_mem_to_mem () //mov @(R3)+, @(R5)+
{
	
	// setup
	reg[3] = 0300;    //ss
	reg[5] = 0500;  //dd
	
	w_write(0300, 0404);
	w_write(0404, 47);
	w_write(0500, 0604);
	w_write(0604, 57);
		
	Command cmd = parse_cmd(0013335);
	
	assert(ss.val == 47);
	assert(ss.adr == 0404);
	assert(dd.val == 57);
	assert(dd.adr == 0604);
	
	cmd.do_command();
	
	assert(reg[3] == 0302);          //проверка, что значение (адресс) в reg[3] увеличилось на 2
	assert(w_read(0300) == 0404);    //проверка, что значение (адресс) в reg[5] не изменился
	assert(w_read(0404) == 47);      //проверка, что число 47 лежит по тому же адрессу
	assert(reg[5] == 0502);
	assert(w_read(0500) == 0604);
	assert(w_read(0604) == 47);
	
	Log(TRACE, "\nfile: %s, line: %u, function: %s is ... OK\n", __FILE__, __LINE__, __FUNCTION__ );
	
	mem_clear();                  //очистка памяти 
	registers_on_null();          //очистка регистров
}

void test_mode4_from_mem_to_reg () //mov R3, -(R5)
{
	
	//setup
	reg[3] = 47;     //ss
	reg[5] = 0500;   //dd
	
	w_write(0476, 57);
	
	Command cmd = parse_cmd(0010345);
	
	assert(ss.val == 47);
	assert(ss.adr == 3);
	assert(dd.val == 57);
	assert(dd.adr == 0476);
	
	cmd.do_command();
	
	assert(reg[3] == 47);        //проверка, что значение (адресс) в reg[3] увеличилось на 2
	assert(w_read(0476) == 47);  //проверка, что изменилось в памяти 
	assert(reg[5] == 00476);     //проверка, что значение (адресс) в reg[5] не изменился
	
	Log(TRACE, "\nfile: %s, line: %u, function: %s is ... OK\n", __FILE__, __LINE__, __FUNCTION__ );
	
	mem_clear();                  //очистка памяти 
	registers_on_null();          //очистка регистров
}

void test_mode4_from_reg_to_mem () //mov -(R3), R5
{
	//setup
	reg[3] = 0300;     //ss
	reg[5] = 57;   //dd
	
	w_write(0276, 47);
	
	Command cmd = parse_cmd(0014305);
	
	assert(ss.val == 47);
	assert(ss.adr == 0276);
	assert(dd.val == 57);
	assert(dd.adr == 5);
	
	cmd.do_command();
	
	assert(reg[3] == 0276);         //проверка, что значение (адресс) в reg[3] увеличилось на 2
	assert(reg[5] == 47);           //проверка, что изменилось в памяти 
	assert(w_read(0276) == 47);     //проверка, что значение (адресс) в reg[5] не изменился
	
	Log(TRACE, "\nfile: %s, line: %u, function: %s is ... OK\n", __FILE__, __LINE__, __FUNCTION__ );
	
	mem_clear();                  //очистка памяти 
	registers_on_null();          //очистка регистров
}

void test_mode4_from_mem_to_mem () //mov -(R3), -(R5)
{
	// setup
	reg[3] = 0152;    //ss
	reg[5] = 0252;  //dd
	
	w_write(0150, 67);
	w_write(0250, 76);
	
	Command cmd = parse_cmd(0014345);
	
	assert(ss.val == 67);
	assert(ss.adr == 0150);
	assert(dd.val == 76);
	assert(dd.adr == 0250);
	
	cmd.do_command();
	
	assert(reg[3] == 0150);   
	assert(reg[5] == 0250);   
	assert(w_read(0250) == 67);
	
	Log(TRACE, "\nfile: %s, line: %u, function: %s is ... OK\n", __FILE__, __LINE__, __FUNCTION__ );
	
	mem_clear();                  //очистка памяти 
	registers_on_null();          //очистка регистров
}

void test_mode5_from_mem_to_reg () //mov R3, @-(R5)
{
	//setup
	reg[3] = 47;     //ss
	reg[5] = 0502;   //dd
	
	w_write(0500, 0600);
	w_write(0600, 57);
	
	Command cmd = parse_cmd(0010355);
	
	assert(ss.val == 47);
	assert(ss.adr == 3);
	assert(dd.val == 57);
	assert(dd.adr == 0600);
	
	cmd.do_command();
	
	assert(reg[3] == 47);         //проверка, что значение в reg[3] не изменилось
	assert(w_read(0500) == 0600); //проверка, что не изменился в памяти 
	assert(w_read(0600) == 47);   //проверка, что в значение поменялось
	assert(reg[5] == 0500);       //проверка, что значение (адресс) в reg[5] изменился
	
	Log(TRACE, "\nfile: %s, line: %u, function: %s is ... OK\n", __FILE__, __LINE__, __FUNCTION__ );
	
	mem_clear();                  //очистка памяти 
	registers_on_null();          //очистка регистров
}

void test_mode5_from_reg_to_mem () //mov @(R3)+, R5
{
	//setup
	reg[3] = 0302;     //ss
	reg[5] = 57;       //dd
	
	w_write(0300, 0400);
	w_write(0400, 47);
	
	Command cmd = parse_cmd(0015305);
	
	assert(ss.val == 47);
	assert(ss.adr == 0400);
	assert(dd.val == 57);
	assert(dd.adr == 5);
	
	cmd.do_command();
	
	assert(reg[3] == 0300);         //проверка, что значение (адресс) в reg[3] увеличилось на 2
	assert(w_read(0300) == 0400);   //проверка, что значение (адресс) в reg[5] не изменился
	assert(w_read(0400) == 47);     //проверка, что число 47 лежит по тому же адрессу
	assert(reg[5] == 47);           //проверка, что изменилось в памяти 
	
	Log(TRACE, "\nfile: %s, line: %u, function: %s is ... OK\n", __FILE__, __LINE__, __FUNCTION__ );
	
	mem_clear();                  //очистка памяти 
	registers_on_null();          //очистка регистров
}

void test_mode5_from_mem_to_mem () //mov @(R3)+, @(R5)+
{
	// setup
	reg[3] = 0302;    //ss
	reg[5] = 0502;    //dd
	
	w_write(0300, 0404);
	w_write(0404, 47);
	w_write(0500, 0604);
	w_write(0604, 57);
		
	Command cmd = parse_cmd(0015355);
	
	assert(ss.val == 47);
	assert(ss.adr == 0404);
	assert(dd.val == 57);
	assert(dd.adr == 0604);
	
	cmd.do_command();
	
	assert(reg[3] == 0300);          //проверка, что значение (адресс) в reg[3] увеличилось на 2
	assert(w_read(0300) == 0404);    //проверка, что значение (адресс) в reg[5] не изменился
	assert(w_read(0404) == 47);      //проверка, что число 47 лежит по тому же адрессу
	assert(reg[5] == 0500);
	assert(w_read(0500) == 0604);
	assert(w_read(0604) == 47);
	
	Log(TRACE, "\nfile: %s, line: %u, function: %s is ... OK\n", __FILE__, __LINE__, __FUNCTION__ );
	
	mem_clear();                  //очистка памяти 
	registers_on_null();          //очистка регистров
}

void test_mode6_from_mem_to_reg() //mov R3, 4(R5)
{
	
	// setup
	reg[3] = 47;      //ss
	reg[5] = 0500;    //dd
	
	pc = 01002;
	w_write(01002, 4);
	w_write(0504, 57);
	
	Command cmd = parse_cmd(0010365); // в режиме тестов мы сами пишем код команды без её разбора в функции read_cmd, где есть pc += 2;
	
	assert(ss.val == 47);
	assert(ss.adr == 3);
	//fprintf(stderr, "\ndd.val = %d dd.adr = %o\n", dd.val, dd.adr);
	assert(dd.val == 57);
	assert(dd.adr == 0504);
	
	cmd.do_command();
	
	assert(w_read(0504) == 47); //проверка на то, что по адресу 500 + 4 лежит число из reg[3]
	
	Log(TRACE, "\nfile: %s, line: %u, function: %s is ... OK\n", __FILE__, __LINE__, __FUNCTION__ );
	
	mem_clear();                  //очистка памяти 
	registers_on_null();          //очистка регистров
}

void test_mode6_from_reg_to_mem() //mov 4(R3), R5
{
	// setup
	reg[3] = 0300;      //ss
	reg[5] = 57;        //dd
	
	pc = 01002;
	w_write(01002, 4);
	w_write(0304, 47);
	
	Command cmd = parse_cmd(0016305); // в режиме тестов мы сами пишем код команды без её разбора в функции read_cmd, где есть pc += 2;
	
	assert(ss.val == 47);
	assert(ss.adr == 0304);
	assert(dd.val == 57);
	assert(dd.adr == 5);
	
	cmd.do_command();
	
	assert(reg[5] == 47); //проверка на то, что в reg[5] лежит число из 300 + 4
	
	Log(TRACE, "\nfile: %s, line: %u, function: %s is ... OK\n", __FILE__, __LINE__, __FUNCTION__ );
	
	mem_clear();                  //очистка памяти 
	registers_on_null();          //очистка регистров
}

void test_mode6_from_mem_to_mem()
{
	// setup
	reg[3] = 0300;      //ss
	reg[5] = 0500;      //dd
	
	pc = 01002;
	w_write(01002, 4);
	w_write(0304, 47);
	w_write(01004, 6);
	w_write(0506, 57);
	
	Command cmd = parse_cmd(0016365); // в режиме тестов мы сами пишем код команды без её разбора в функции read_cmd, где есть pc += 2;
	
	assert(ss.val == 47);
	assert(ss.adr == 0304);
	assert(dd.val == 57);
	assert(dd.adr == 0506);
	
	cmd.do_command();
	
	assert(w_read(0506) == 47); //проверка на то, что по адресу 500 + 6 лежит число из адреса 300 + 4
	
	Log(TRACE, "\nfile: %s, line: %u, function: %s is ... OK\n", __FILE__, __LINE__, __FUNCTION__ );
	
	mem_clear();                  //очистка памяти 
	registers_on_null();          //очистка регистров
}
