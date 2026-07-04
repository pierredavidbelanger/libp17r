#include "p17r-app.h"
#include <raylib.h>

int app_main(const int argc, char *argv[],
             const app_init_func init,
             const app_input_func input, const app_tick_func tick, const app_frame_func frame,
             const app_fini_func fini) {
    app_t app = {0};
    app.tps = 30;
    app.fps = 60;
    app.running = true;

    InitWindow(800, 600, "");

    int rc = init(&app, argc, argv);
    if (rc != 0) {
        return fini(&app, rc);
    }

    double tickLast = GetTime();
    float tickAcc = 0;
    int fps = 0;

    while (app.running) {
        if (WindowShouldClose()) {
            rc = 1;
            break;
        }

        rc = input(&app);
        if (rc != 0) {
            break;
        }

        if (app.tps > 0) {
            const double now = GetTime();
            float dt = (float) (now - tickLast);
            tickLast = now;
            if (dt > 0.1f) {
                dt = 0.1f;
            }
            tickAcc += dt;
            const float tickFreq = 1.0f / (float) app.tps;
            while (tickAcc >= tickFreq) {
                rc = tick(&app, tickFreq);
                if (rc != 0) {
                    break;
                }
                tickAcc -= tickFreq;
            }
            if (rc != 0) {
                break;
            }
        }

        if (app.fps != fps) {
            fps = app.fps;
            SetTargetFPS(fps);
        }
        rc = frame(&app);
        if (rc != 0) {
            break;
        }
    }

    rc = fini(&app, rc);

    CloseWindow();

    return rc;
}
