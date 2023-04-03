#include "run.h"

extern word reg[REGSIZE];

Arg get_mr(word w);

Arg ss, dd;

void do_halt()
{
    //Log(INFO, "THE END!!!\n");
	reg_dump();
    registers_on_null();
	exit(0);
}
void do_add() 
{
	w_write(dd.adr, ss.val + dd.val);
}
void do_mov() 
{
	w_write(dd.adr, ss.val);
}
void do_sob()
{
	
}
void do_inc()
{
	
}
void do_nothing() 
{
	
}

Command command[] = {
    {0170000, 0060000, "add",  do_add},
	{0170000, 0010000, "mov",  do_mov},
	{0177777, 0000000, "halt", do_halt},
	{0177700, 0005200, "inc",  do_inc},
	{0177000, 0077000, "sob",  do_sob},
	{0000000, 0000000, "unknown", do_nothing} //всегда в конце массива!!!
};

Arg get_mr(word w)
{
    Arg res;
	int r = w & 7;          // номер регистра
	int m = (w >> 3) & 7;   // номер моды
    switch (m) {
    // мода 0, R1
    case 0:
        res.adr = r;        // адрес - номер регистра
        res.val = reg[r];   // значение - число в регистре
        Log(TRACE, "R%d ", r);
        break;
    // мода 1, (R1)
    case 1:
        res.adr = reg[r];           // в регистре адрес
        res.val = w_read(res.adr);  // по адресу - значение
        Log(TRACE, "(R%d) ", r);
        break;
    // мода 2, (R1)+ или #3
    case 2:
        res.adr = reg[r];           // в регистре адрес
        res.val = w_read(res.adr);  // по адресу - значение
        reg[r] += 2;                // TODO: +1
        // печать разной мнемоники для PC и других регистров
        if (r == 7)
            Log(TRACE, "#%o ", res.val);
        else
            Log(TRACE, "(R%d)+ ", r);
        break;


    // мы еще не дописали другие моды
    default:
        Log(ERROR, "Mode %d not implemented yet!\n", m);
        //exit(1);
		break;
	}
	return res;
}

void reg_dump()
{
	for(int i = 0; i < 8; i++)
		Log(TRACE, "r%d:%o ", i, reg[i]);
	Log(TRACE, "\n");
}

word read_cmd()
{
	word w;
	w = w_read(pc);
	Log(TRACE, "%06o %06o: ", pc, w);
	pc += 2;
	return w;
}

Command parse_cmd(word w)
{
	for (int i = 0; ; i++) {
		if ((w & command[i].mask) == command[i].opcode) {
			Log(TRACE, "%s ", command[i].name);
			if (command[i].mask != 0177777) { //halt не должен печатать R0 R0!!!
				dd = get_mr(w);
				ss = get_mr(w >> 6);
			}
			else
				Log(TRACE, "\n");
			return command[i];
		}
	}
}

void registers_on_null()
{
	for(int i = 0; i < 8; i++)
		reg[i] = 0;
}

void run()
{
    // следующее слово будем читать по адресу 1000 (восьмеричное)
	pc = 01000;	
	
	Command cmd;
	while(1) {
		cmd = parse_cmd(read_cmd()); // читаем слово и разбираем команду
		cmd.do_command();            // выполняем команду
		Log(TRACE,"\n");
	}
}
