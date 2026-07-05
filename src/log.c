#include <stdio.h>

#include "p17r-log.h"

#if SUPPORT_TRACELOG
    #include <raylib.h>
    #include <stdarg.h>
#endif

void p17r_tracelog_internal(int level, const char *text, ...) {
#if SUPPORT_TRACELOG
    // not really efficient, but whatever
    char buffer[1024];
    va_list args;
    va_start(args, text);
    vsnprintf(buffer, sizeof(buffer), text, args);
    va_end(args);
    TraceLog(level, "%s", buffer);
#endif
}
