#ifndef P17R_NOISE_H
#define P17R_NOISE_H

// See https://auburn.github.io/FastNoiseLite/
#include <FastNoiseLite.h>

#ifdef __cplusplus
extern "C" {
#endif

float fnlGetNoise2DWrap(const fnl_state *state, FNLfloat x, FNLfloat y, FNLfloat scale_x, FNLfloat scale_y, FNLfloat w, FNLfloat h);

#ifdef __cplusplus
}
#endif

#endif //P17R_NOISE_H
