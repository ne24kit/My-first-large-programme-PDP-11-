
#include "test_reg.h"

extern Arg ss, dd;

extern Command command[];

extern word reg[REGSIZE];

void tests_on_cmd()
{
	test_parse_mov();
	test_mode0();
	test_mov();
	Log(ERROR, "Тесты на определение команд пройдены\n");
	registers_on_null();
}

// тест, что мы правильно определяем команды mov, add, halt
void test_parse_mov()
{
    Log(TRACE, "file: %s, line: %u, function: %s\n", __FILE__, __LINE__, __FUNCTION__ );
    Command cmd = parse_cmd(0017654);
    assert(!strcmp(cmd.name, "mov"));
    Log(TRACE, " ... OK\n");
}
// тест, что мы разобрали правильно аргументы ss и dd в mov R5, R3
void test_mode0()
{
    Log(TRACE, "file: %s, line: %u, function: %s\n", __FILE__, __LINE__, __FUNCTION__ );
    reg[3] = 012;    // dd
    reg[5] = 034;    // ss
	//						   ssdd
    Command cmd = parse_cmd(0010503);
	
    assert(ss.val == 034);
    assert(ss.adr == 5);
    assert(dd.val == 012);
    assert(dd.adr == 3);
    Log(TRACE, " ... OK\n");
}
// тест, что mov и мода 0 работают верно в mov R5, R3
void test_mov()
{
    Log(TRACE, "file: %s, line: %u, function: %s\n", __FILE__, __LINE__, __FUNCTION__ );
    reg[3] = 012;    // dd
    reg[5] = 034;    // ss
    Command cmd = parse_cmd(0010503);
    cmd.do_command();
    assert(reg[3] == 034);
    assert(reg[5] == 034);
    Log(TRACE, " ... OK\n");
}

void test_mode1_toreg()
{
    Log(TRACE, "file: %s, line: %u, function: %s\n", __FILE__, __LINE__, __FUNCTION__ );


    // setup
    reg[3] = 12;    // dd
    reg[5] = 0200;  // ss
    w_write(0200, 34);


    Command cmd = parse_cmd(0011503);


    assert(ss.val == 34);
    assert(ss.adr == 0200);
    assert(dd.val == 12);
    assert(dd.adr == 3);


    cmd.do_command();


    assert(reg[3] = 34);
    // проверяем, что значение регистра не изменилось
    assert(reg[5] = 0200);


    trace(TRACE, " ... OK\n");
}
