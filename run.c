#include "run.h"

extern word reg[REGSIZE];

Arg get_mr(word w);

Arg ss, dd, rnn;

word flag_b_cmd;

word is_byte_cmd(word w)
{
	if (w & 0100000)
		flag_b_cmd = 1;
	else
		flag_b_cmd = 0;
	return  w;
}

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
void do_movb()
{
	b_write(dd.adr, ss.val);
}
void do_sob()
{
	if (--reg[rnn.adr] == 0)
		return;
	pc -= 2*rnn.val;
}
void do_inc()
{
	
}
void do_nothing() 
{
	
}

Command command[] = {
    {0170000, 0060000, "add",  do_add, HAS_SS | HAS_DD},
	{0170000, 0010000, "mov",  do_mov, HAS_SS | HAS_DD},
	{0170000, 0110000, "movb", do_movb, HAS_SS | HAS_DD},
	{0177777, 0000000, "halt", do_halt, NO_PARAMS},
	{0177700, 0005200, "inc",  do_inc, HAS_DD},
	{0177000, 0077000, "sob",  do_sob, HAS_NN | HAS_R}, 
	{0000000, 0000000, "unknown", do_nothing, NO_PARAMS} //всегда в конце массива!!!
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
		if (flag_b_cmd) {
			res.val = b_read(res.adr);
			reg[r]++;
		}
		if (((r == 7) || (r == 6)) && (flag_b_cmd))
			reg[r]++; 		
		if(!flag_b_cmd) {
			res.val = w_read(res.adr);  // по адресу - значение
			reg[r] += 2;
		}
        // печать разной мнемоники для PC и других регистров
        if (r == 7)
            Log(TRACE, "#%o ", res.val);
        else
            Log(TRACE, "(R%d)+ ", r);
        break;
	// мода 3, @(R1)+
	case 3:
		res.adr = w_read(reg[r]);
		res.val = w_read(res.adr);
		reg[r] += 2;
		// печать разной мнемоники для PC и других регистров
		if (r == 7)
			Log(TRACE, "@#%o ", res.val);
		else
			Log(TRACE, "@(R%d)+ ", r);
		break;
	// мода 4, -(Rn)
	case 4:
		if(!flag_b_cmd){
			reg[r] -= 2;
			res.adr = reg[r];
			res.val = w_read(res.adr);
		}
		if(flag_b_cmd) {
			reg[r]--;
			res.adr = reg[r];
			res.val = b_read(res.adr);
		}
		if (((r == 7) || (r == 6)) && (flag_b_cmd))
			reg[r]--;
		Log(TRACE, "-(R%d) ", r);
		break;
	// мода 5, @-(Rn)
	case 5:
		reg[r] -= 2;
		res.adr = w_read(reg[r]);
		res.val = w_read(res.adr);
		Log(TRACE, "@-(R%d) ", r);
		break;
		
    // мы еще не дописали другие моды
    default:
        Log(ERROR, "\nMode %d not implemented yet!\n", m);
        //exit(1);
		break;
	}
	return res;
}

Arg get_rnn(word w)
{
	Arg res;
	int r = (w >> 6) & 7;
	int nn = w & 077;
	res.adr = r;  // адрес 
	res.val = nn; // смещение
	Log(TRACE, "R%d %06o ", r, pc - 2*nn);
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
		w = is_byte_cmd(w);
		if ((w & command[i].mask) == command[i].opcode) {
			Log(TRACE, "%s ", command[i].name);
			if (command[i].mask != 0177777) { //halt не должен печатать R0 R0!!!
				if (command[i].params & HAS_SS)
					ss = get_mr(w >> 6);
				if (command[i].params & HAS_DD)
					dd = get_mr(w);
				if (command[i].params & (HAS_NN | HAS_R))
					rnn = get_rnn(w);
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
