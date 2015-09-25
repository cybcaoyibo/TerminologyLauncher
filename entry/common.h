#ifndef ENTRY_COMMON_H
#define ENTRY_COMMON_H

#include <uv.h>
#undef RB_RED
#undef RB_BLACK
#include "../lcsc/vector.h"
#include "../lcsc/lcsc_string.h"
#include "../csl2/misc.h"
#include <inttypes.h>

void fatal(const char *msg);
void afail(const char *file, int line);
#define FAIL() {afail(__FILE__, __LINE__);}
void staticInit();
void staticFree();
int localtime_r(time_t *stamp, struct tm *info);

#endif


