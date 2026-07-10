#include "p17r-mem.h"

#include "p17r-profile.h"
#include "p17r-log.h"
#include "p17r-rl.h"

#define ARENA_IMPLEMENTATION
#include <arena.h>

#include <stdlib.h>

void *p17r_malloc_internal(const size_t sz) {
    TRACELOG(LOG_DEBUG, "malloc(%lld)", sz);
    void *ptr = malloc(sz);
    #ifdef TRACY_ENABLE
    if (ptr) {
        TracyCAlloc(ptr, sz);
    }
    #endif
    return ptr;
}

void *p17r_calloc_internal(const size_t n, const size_t sz) {
    TRACELOG(LOG_DEBUG, "calloc(%lld, %lld)", n, sz);
    void *ptr = calloc(n, sz);
    #ifdef TRACY_ENABLE
    if (ptr) {
        TracyCAlloc(ptr, n * sz);
    }
    #endif
    return ptr;
}

void *p17r_realloc_internal(void *ptr, const size_t sz) {
    TRACELOG(LOG_DEBUG, "realloc(%p, %lld)", ptr, sz);
    #ifdef TRACY_ENABLE
    if (ptr) {
        TracyCFree(ptr);
    }
    #endif
    void *new_ptr = realloc(ptr, sz);
    #ifdef TRACY_ENABLE
    if (new_ptr) {
        TracyCAlloc(new_ptr, sz);
    }
    #endif
    return new_ptr;
}

void p17r_free_internal(void *ptr) {
    TRACELOG(LOG_DEBUG, "free(%p)", ptr);
    #ifdef TRACY_ENABLE
    if (ptr) {
        TracyCFree(ptr);
    }
    #endif
    free(ptr);
}
