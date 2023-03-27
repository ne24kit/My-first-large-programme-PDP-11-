#pragma once

#include <stdio.h>
#include <stdarg.h>

enum LogGING_LEVELS {
	ERROR,
	INFO,
	TRACE,
	DEBUG
};

void Log(int level, const char * format, ...);

int set_Log_level(int level);
