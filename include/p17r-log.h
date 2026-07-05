#ifndef P17R_LOG_H
#define P17R_LOG_H

#ifndef SUPPORT_TRACELOG
    #ifdef NDEBUG
        #define SUPPORT_TRACELOG 0
    #else
        #define SUPPORT_TRACELOG 1
    #endif
#endif

#if SUPPORT_TRACELOG
    void p17r_tracelog_internal(int level, const char *text, ...);
    #define TRACELOG(level, ...) p17r_tracelog_internal(level, __VA_ARGS__)
#else
    #define TRACELOG(level, ...) (void)0
#endif

#endif //P17R_LOG_H
