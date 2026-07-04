#ifndef P17R_MEM_H
#define P17R_MEM_H
#if P17R_DEBUG

#include <stdlib.h>
#include <stdio.h>

#ifndef P17R_MALLOC
#define P17R_MALLOC(sz)      malloc(sz);printf("P17R_MALLOC(%d)\n",sz)
#endif
#ifndef P17R_CALLOC
#define P17R_CALLOC(n,sz)    calloc(n,sz);printf("P17R_CALLOC(%d,%d)\n",n,sz)
#endif
#ifndef P17R_REALLOC
#define P17R_REALLOC(ptr,sz) realloc(ptr,sz);printf("P17R_REALLOC(*,%d)\n",sz)
#endif
#ifndef P17R_FREE
#define P17R_FREE(ptr)       free(ptr);printf("P17R_FREE(*)\n")
#endif

#else

#include <stdlib.h>

#ifndef P17R_MALLOC
#define P17R_MALLOC(sz)      malloc(sz)
#endif
#ifndef P17R_CALLOC
#define P17R_CALLOC(n,sz)    calloc(n,sz)
#endif
#ifndef P17R_REALLOC
#define P17R_REALLOC(ptr,sz) realloc(ptr,sz)
#endif
#ifndef P17R_FREE
#define P17R_FREE(ptr)       free(ptr)
#endif

#endif
#endif //P17R_MEM_H
