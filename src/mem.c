#include "p17r-mem.h"

#include "p17r-log.h"

#include <raylib.h>

#define ARENA_IMPLEMENTATION
#include <arena.h>

#include <stdlib.h>

void *p17r_malloc_internal(const size_t sz) {
    TRACELOG(LOG_DEBUG, "malloc(%lld)", sz);
    return malloc(sz);
}

void *p17r_calloc_internal(const size_t n, const size_t sz) {
    TRACELOG(LOG_DEBUG, "calloc(%lld, %lld)", n, sz);
    return calloc(n, sz);
}

void *p17r_realloc_internal(void *ptr, const size_t sz) {
    TRACELOG(LOG_DEBUG, "realloc(%p, %lld)", ptr, sz);
    return realloc(ptr, sz);
}

void p17r_free_internal(void *ptr) {
    TRACELOG(LOG_DEBUG, "free(%p)", ptr);
    free(ptr);
}
