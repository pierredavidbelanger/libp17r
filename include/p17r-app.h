#ifndef P17R_APP_H
#define P17R_APP_H

#include <stdbool.h>

typedef struct app_t {
    void *state;
    int tps;
    int fps;
    bool running;
} app_t;

typedef int (*app_init_func)(app_t *app, int argc, char *argv[]);

typedef int (*app_input_func)(app_t *app);

typedef int (*app_tick_func)(app_t *app, float dt);

typedef int (*app_frame_func)(app_t *app);

typedef int (*app_fini_func)(app_t *app, int rc);

int app_main(int argc, char *argv[],
             app_init_func init,
             app_input_func input, app_tick_func tick, app_frame_func frame,
             app_fini_func fini);

#endif //P17R_APP_H
