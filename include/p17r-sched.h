#ifndef P17R_SCHED_H
#define P17R_SCHED_H

#include <stdbool.h>

typedef struct sched_t {
    float freq;
    double last;
    double acc;
} sched_t;

#ifdef __cplusplus
extern "C" {
#endif

sched_t sched_create(int tps);

void sched_set_tps(sched_t *s, int tps);

float sched_begin(sched_t *s);

bool sched_has_hext(const sched_t *s);

void sched_next(sched_t *s);

#define sched_loop(dt, s) for (const float dt = sched_begin(s); sched_has_hext(s); sched_next(s))

#ifdef __cplusplus
}
#endif

#endif //P17R_SCHED_H
