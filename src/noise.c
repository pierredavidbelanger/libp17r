#include "p17r-noise.h"
#include "p17r-mem.h"
#include <open-simplex-noise.c>

struct osn_t {
    struct osn_context *ctx;
};

osn_t *osn_create(const long long seed) {
    osn_t *osn = P17R_CALLOC(1, sizeof(osn_t));
    open_simplex_noise(seed, &osn->ctx);
    return osn;
}

void osn_free(osn_t *osn) {
    open_simplex_noise_free(osn->ctx);
    P17R_FREE(osn);
}

double osn_noise2d_norm(const osn_t *osn,
                        const double x, const double y,
                        const double w, const double h,
                        const double scale) {
    const OSNFLOAT sx = x / w * scale;
    const OSNFLOAT sy = y / h * scale;
    const OSNFLOAT result = open_simplex_noise2(osn->ctx, sx, sy);
    return (OSNFLOAT)((1.0 + result) / 2.0);
}

double osn_noise3d_norm(const osn_t *osn,
                        const double x, const double y, const double z,
                        const double w, const double h, const double d,
                        const double scale) {
    const OSNFLOAT sx = x / w * scale;
    const OSNFLOAT sy = y / h * scale;
    const OSNFLOAT sz = z / d * scale;
    const OSNFLOAT result = open_simplex_noise3(osn->ctx, sx, sy, sz);
    return (OSNFLOAT)((1.0 + result) / 2.0);
}
