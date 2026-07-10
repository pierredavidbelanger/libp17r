#include "p17r-log.h"

#if SUPPORT_TRACELOG
    #include "p17r-rl.h"
#endif

#if SUPPORT_TRACELOG
    #include <stdio.h>
    #include <stdarg.h>
#endif

#if SUPPORT_TRACELOG
void p17r_tracelog_internal(int level, const char *text, ...) {
    // not really efficient, but whatever
    char buffer[1024];
    va_list args;
    va_start(args, text);
    vsnprintf(buffer, sizeof(buffer), text, args);
    va_end(args);
    TraceLog(level, "%s", buffer);
}
#else
void p17r_tracelog_internal(void) {
}
#endif
