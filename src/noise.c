#include "p17r-noise.h"
#include "p17r-mem.h"
#include <math.h>
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

double osn_noise2d_norm(const osn_t *osn, const double x, const double y, const double w, const double h, const double scale) {
    const OSNFLOAT sx = x / w * scale;
    const OSNFLOAT sy = y / h * scale;
    const OSNFLOAT result = open_simplex_noise2(osn->ctx, sx, sy);
    return (double) (((OSNFLOAT) 1.0 + result) / (OSNFLOAT) 2.0);
}

double osn_noise2d_wrap_norm(const osn_t *osn, const double x, const double y, const double scale_x, const double scale_y, const double w, const double h, const unsigned int octaves, const double lacunarity, const double gain) {
    OSNFLOAT result = (OSNFLOAT) 0.0;
    // Value and max theoretical amplitude
    OSNFLOAT result_value = (OSNFLOAT) 0.0;
    OSNFLOAT result_max = (OSNFLOAT) 0.0;
    // High-frequency layer accumulator
    OSNFLOAT frequency = (OSNFLOAT) 1.0;
    OSNFLOAT amplitude = (OSNFLOAT) 1.0;
    // Wrapping angular coordinates
    const OSNFLOAT rad_x = ((OSNFLOAT) 2.0 * (OSNFLOAT) M_PI * x) / w;
    const OSNFLOAT rad_y = ((OSNFLOAT) 2.0 * (OSNFLOAT) M_PI * y) / h;
    // Base scaled unit circles on the 4D plane
    const OSNFLOAT cos_x = cos(rad_x) / scale_x;
    const OSNFLOAT sin_x = sin(rad_x) / scale_x;
    const OSNFLOAT cos_y = cos(rad_y) / scale_y;
    const OSNFLOAT sin_y = sin(rad_y) / scale_y;
    for (unsigned int i = 0; i < octaves; ++i) {
        // Sample raw 4D OpenSimplex noise scaled by current octave frequency
        OSNFLOAT n = open_simplex_noise4(osn->ctx,
                                         cos_x * frequency, sin_x * frequency,
                                         cos_y * frequency, sin_y * frequency);
        // Ridged transformation
        if (i > 0) {
            n = (OSNFLOAT) 1.0 - (OSNFLOAT) fabs((double) n);
            // Ridge peaks pull up
            n = n * n;
        }
        // Accumulate fractal layers
        result_value += n * amplitude;
        result_max += amplitude;
        // Shift frequency and amplitude for next high-frequency layer
        frequency *= lacunarity;
        amplitude *= gain;
    }
    // Normalized value relative to the max theoretical amplitude
    result = result_value / result_max;
    return (double) (((OSNFLOAT) 1.0 + result) / (OSNFLOAT) 2.0);
    // return (double) result;
}

double osn_noise3d_norm(const osn_t *osn, const double x, const double y, const double z, const double w, const double h, const double d, const double scale) {
    const OSNFLOAT sx = x / w * scale;
    const OSNFLOAT sy = y / h * scale;
    const OSNFLOAT sz = z / d * scale;
    const OSNFLOAT result = open_simplex_noise3(osn->ctx, sx, sy, sz);
    return (double) (((OSNFLOAT) 1.0 + result) / (OSNFLOAT) 2.0);
}
