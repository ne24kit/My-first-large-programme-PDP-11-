#include "run.h"

extern word reg[REGSIZE];

void do_halt()
{
    Log(INFO, "THE END!!!\n");
    exit(0);
}
void do_add() 
{
	
}
void do_mov() 
{
	
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


void run()
{
    // следующее слово будем читать по адресу 1000 (восьмеричное)
    pc = 01000;
	
    word w;
    
    while(1) {
        w = w_read(pc);
        Log(TRACE, "%06o %06o: ", pc, w);
        pc += 2;
		for (int i = 0; ; i++) {
			if ((w & command[i].mask) == command[i].opcode) {
				Log(TRACE, "%s ", command[i].name);
				command[i].do_command();
				break;
			}
		}
		Log(TRACE,"\n");
		
	}
}
