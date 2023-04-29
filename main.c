#include "header.h"

extern int level;

int main(int argc, char * argv[])
{	
	
	set_Log_level(TRACE);
	
	if (level == DEBUG) {
		test_mem();
		tests_on_cmd();
	}
	
	set_ostat();
	
	if(argc == 1 || argc == 2) {
		how_to_use_keys(argv[0]);
		exit(1);
	}
	
	load_file(argv[2]);
		
	run();
	
	return 0;
}

void load_file(const char * filename)
{	
	FILE * fin  = fopen(filename, "r");   // открыть файл filename на чтение - поток fin
	if (fin == NULL) {
		perror("ERROR");
		Log(ERROR, "FILE:  %s\n", filename);
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

void how_to_use_keys(const char * progname)
{
	printf("to compile use: %s [-t] <filename>\n	<filename> - <input data>\n", progname);
}