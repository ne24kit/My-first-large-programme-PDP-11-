#include "log.h"

int level;

int set_Log_level(int new_level)
{
	
	int old_level = level;
	level = new_level;
	
	return old_level;
}

void Log(int Log_level, const char * format, ...)
{
	if (level >= Log_level) {
		va_list ap;
		va_start(ap, format);
		vfprintf(stdout, format, ap);
		va_end(ap);
	}
}