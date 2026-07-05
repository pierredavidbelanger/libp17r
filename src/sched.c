#include "p17r-sched.h"

#include <raylib.h>

#include <math.h>

#define MIN(a,b) (((a)<(b))?(a):(b))

sched_t sched_create(const int tps) {
    sched_t s = {0};
    sched_set_tps(&s, tps);
    return s;
}

void sched_set_tps(sched_t *s, const int tps) {
    s->freq = tps <= 0 ? 0.0f : 1.0f / (float) tps;
}

float sched_begin(sched_t *s) {
    const double curr = GetTime();
    // cap lag spikes at 1/4 of a second
    s->acc += fminf(0.25f, (float) (curr - s->last));
    s->last = curr;
    return s->freq;
}

bool sched_has_hext(const sched_t *s) {
    if (s->freq <= 0.0f) return false;
    return s->acc >= s->freq;
}

void sched_next(sched_t *s) {
    if (s->freq <= 0.0f) return;
    s->acc -= s->freq;
}
