#include "run.h"

extern word reg[REGSIZE];

byte flag_N;
byte flag_Z;
byte flag_V;
byte flag_C;

Arg get_mr(word w);

Arg ss, dd, rnn, reg_num;

signed char xx;

word flag_b_cmd;

void dumb_flags()
{
	Log(TRACE, "\nN = %d Z = %d V = %d C = %d \n", flag_N, flag_Z, flag_V, flag_C);
}

void set_NZ(word w)
{
	flag_Z = w == 0; //флаг Z = 0, если w = 1
	if (flag_b_cmd)
		flag_N = (w >> 7) & 1;
	else
		flag_N = (w >> 15) & 1;
}

void set_C(unsigned int w)
{
	if (flag_b_cmd)
		flag_C = (w >> 8) & 1;
	else 
		flag_C = (w >> 16) & 1;
}

void is_byte_cmd(word w)
{
	flag_b_cmd = (w >> 15) & 1;
}

void do_jsr() //есть dd и reg_num - номер используемого регистра (reg_num.val не используется)
{
	sp -= 2;
	w_write(sp, reg[reg_num.adr]);
	reg[reg_num.adr] = pc;
	pc = dd.adr;
}

void do_rts()
{
	pc = reg[reg_num.adr];
	reg[reg_num.adr] = w_read(sp);
	sp += 2;
}

void do_tst() //выставляет флаги N и Z, обнуляя V и С
{
	set_NZ(dd.val);
	flag_V = 0;
	flag_C = 0;
}

void do_clr()
{
	if (flag_b_cmd)
		b_write(dd.adr, 0);
	else
		w_write(dd.adr, 0);
	
	flag_N = 0;
	flag_Z = 1;
	flag_V = 0;
	flag_C = 0;
}

void do_br() //- не изменяет флаги
{
	pc += 2*xx;
}

void do_bcc() //Branch if Carry Clear
{
	if (flag_C == 0)
		do_br();
}

void do_bcs() //Branch if Carry Set
{
	if (flag_C == 1)
		do_br();
}

void do_beq() //Branch if Equal
{
	if (flag_Z == 1)
		do_br();
}

void do_bne() //Branch if Not Equal
{
	if (flag_Z == 0)
		do_br();
}
void do_bmi() //Branch if Minus
{
    if (flag_N == 1)
        do_br();
}

void do_bpl() //Branch if Plus
{
	if (flag_N == 0) 
        do_br();
}

void do_cmp() //вычисляет разность аргументов ss-dd и по этой разности выставляет все флаги
{
	unsigned int res = ss.val;
	res -= dd.val;
	set_NZ(res);
	set_C(res);
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
	unsigned int res = ss.val;
	res += dd.val;
	set_NZ(res);
	set_C(res);
}
void do_mov() 
{
	w_write(dd.adr, ss.val);
	set_NZ(ss.val);
	flag_V = 0;
}
void do_movb()
{
	b_write(dd.adr, ss.val);
	set_NZ(ss.val);
	flag_V = 0;
}
void do_sob()
{
	if (--reg[rnn.adr] == 0)
		return;
	pc -= 2*rnn.val;
}

void do_nothing() 
{
	
}

Command command[] = {
    {0170000, 0060000, "add",  do_add, HAS_SS | HAS_DD},
	{0170000, 0010000, "mov",  do_mov, HAS_SS | HAS_DD},
	{0170000, 0110000, "movb", do_movb, HAS_SS | HAS_DD},
	{0177777, 0000000, "halt", do_halt, NO_PARAMS},
	{0177000, 0077000, "sob",  do_sob, HAS_NN | HAS_R},
	{0177000, 0004000, "jsr",  do_jsr, HAS_DD | HAS_R},
	{0177770, 0000200, "rts",  do_rts, HAS_R},
	{0177700, 0005700, "tst",  do_tst, HAS_DD},
	{0177700, 0105700, "tstb", do_tst, HAS_DD},
	{0177400, 0000400, "br",   do_br,  HAS_XX},
	{0177400, 0103000, "bcc",  do_bcc, HAS_XX},
	{0177400, 0103400, "bcs",  do_bcs, HAS_XX},
	{0177400, 0001400, "beq",  do_beq, HAS_XX},
	{0177400, 0001000, "bne",  do_bne, HAS_XX},
	{0177400, 0100000, "bpl",  do_bpl, HAS_XX},
	{0177400, 0100400, "bmi",  do_bmi, HAS_XX},
	{0177000, 0005000, "clr",  do_clr, HAS_DD},
	{0170000, 0020000, "cmp",  do_cmp, HAS_SS | HAS_DD},
	{0170000, 0120000, "cmpb", do_cmp, HAS_SS | HAS_DD},
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
	// мода 6, x(Rn)
	case 6:
		{
		word x = w_read(pc); //смещение
		//fprintf(stderr, "\nx = %d\n", x);
		pc += 2;
		res.adr = reg[r] + x; //записали
		res.val = w_read(res.adr);
		if (r == 7)
			Log(TRACE, "%o(pc) ", x);
		else
			Log(TRACE, "%o(R%d) ", x, r);
		}
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

signed char get_xx(word w)
{
	Log(TRACE,"to: %o ", pc+2*((signed char)w));
	return w; //знаковое, то есть можно прыгать как вперед, так и назад!!!
}

Arg get_r(word w)
{
	Arg res;
	res.adr = w & 7;
	if (res.adr == 7)
		Log(TRACE, "pc ");
	else
		Log(TRACE, "R%d ", res.adr);
	return res;
}

void reg_dump()
{
	dumb_flags();
	for(int i = 0; i < REGSIZE - 2; i++)
		Log(TRACE, "r%d:%o ", i, reg[i]);
	Log(TRACE, "sp:%o ", sp);
	Log(TRACE, "pc:%o ", pc);
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
		is_byte_cmd(w);
		if ((w & command[i].mask) == command[i].opcode) {
			Log(TRACE, "%s ", command[i].name);
			//dumb_flags();
			if (command[i].mask != 0177777) { //halt не должен печатать R0 R0!!!
				if (command[i].params == (HAS_NN | HAS_R))
					rnn = get_rnn(w);
				if (command[i].params == (HAS_DD | HAS_R)) //разбор у jsr reg_num
					reg_num = get_r(w >> 6);
				if (command[i].params == (HAS_R))
					reg_num = get_r(w);
				if (command[i].params & HAS_SS)
					ss = get_mr(w >> 6);
				if (command[i].params & HAS_DD)
					dd = get_mr(w);
				if (command[i].params & (HAS_XX))
					xx = get_xx(w);
			}
			else
				Log(TRACE, "\n");
			return command[i];
		}
	}
}

void registers_on_null()
{
	for(int i = 0; i < REGSIZE; i++)
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
