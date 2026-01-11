#include <stdarg.h>
#include <stdio.h>

#include "logging.h"

void _LogWarning(const char* funcName, i32 line, const char* format, ...) {
	printf("[Warning] (%s: line %d) ", funcName, line);
	va_list args;
	va_start(args, format);
	vprintf(format, args);
	va_end(args);
	printf("\n");
}

void _LogError(const char* funcName, i32 line, const char* format, ...) {
	printf("[Error] (%s: line %d) ", funcName, line);
	va_list args;
	va_start(args, format);
	vprintf(format, args);
	va_end(args);
	printf("\n");
}