#pragma once
#include "../general.h"

void _LogWarning(const char* funcName, i32 line, const char* format, ...);
void _LogError(const char* funcName, i32 line, const char* format, ...);

#define LogWarning(format, ...) _LogWarning(__FUNCTION__, __LINE__, format, ##__VA_ARGS__)
#define LogError(format, ...) _LogError(__FUNCTION__, __LINE__, format, ##__VA_ARGS__)