#ifndef P17R_MEM_H
#define P17R_MEM_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stdlib.h>

#ifndef P17R_MALLOC
    void *p17r_malloc_internal(size_t sz);
    #define P17R_MALLOC(sz) p17r_malloc_internal(sz)
#endif

#ifndef P17R_CALLOC
    void *p17r_calloc_internal(size_t n, size_t sz);
    #define P17R_CALLOC(n, sz) p17r_calloc_internal(n, sz)
#endif

#ifndef P17R_REALLOC
    void *p17r_realloc_internal(void *ptr, size_t sz);
    #define P17R_REALLOC(ptr, sz) p17r_realloc_internal(ptr, sz)
#endif

#ifndef P17R_FREE
    void p17r_free_internal(void *ptr);
    #define P17R_FREE(ptr) p17r_free_internal(ptr)
#endif

#ifdef __cplusplus
}
#endif

#endif //P17R_MEM_H
