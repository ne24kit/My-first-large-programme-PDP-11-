#include <stdio.h>

#include "mem.h"
#include "test_mem.h"
#include "test_reg.h"
#include "run.h"
#include "log.h"

int main(int argc, char * argv[])
{	
	set_Log_level(DEBUG);
	
	test_mem();
	tests_on_cmd();

	if(argc == 1 || argc == 2) {
		how_to_use_keys(argv[0]);
		exit(1);
	}
	
	const char * filename = argv[argc-1];
	
	char trace = 0;
	
	if (argc == 3 && (0 == strcmp("-t", argv[1])))
		trace = 1;
	
	load_file(argv[2]);
	
	//mem_dump(0x0200,  0x000c);
	
	
	run();
	
	return 0;
}
