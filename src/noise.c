#include "p17r-noise.h"

#define FNL_IMPL
#include <FastNoiseLite.h>

#include <math.h>

float fnlGetNoise2DWrap(const fnl_state *state, const FNLfloat x, const FNLfloat y, const FNLfloat scale_x, const FNLfloat scale_y, const FNLfloat w, const FNLfloat h) {
    // Normalize coordinates to [0, 1] interval
    const FNLfloat u = x / w;
    const FNLfloat v = y / h;
    // Ensure coordinates wrap within the tile range [0, w] and [0, h]
    FNLfloat rx = fmodf(x, w);
    if (rx < 0) rx += w;
    FNLfloat ry = fmodf(y, h);
    if (ry < 0) ry += h;
    // Sample the 4 corners of the wrapping boundary
    const FNLfloat n00 = fnlGetNoise2D(state, rx * scale_x, ry * scale_y);
    const FNLfloat n10 = fnlGetNoise2D(state, (rx - w) * scale_x, ry * scale_y);
    const FNLfloat n01 = fnlGetNoise2D(state, rx * scale_x, (ry - h) * scale_y);
    const FNLfloat n11 = fnlGetNoise2D(state, (rx - w) * scale_x, (ry - h) * scale_y);
    // Interpolate using the smooth curve
    const FNLfloat n0 = n00 + u * (n10 - n00);
    const FNLfloat n1 = n01 + u * (n11 - n01);
    return n0 + v * (n1 - n0);
}
