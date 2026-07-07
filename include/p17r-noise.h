#ifndef P17R_NOISE_H
#define P17R_NOISE_H

// See https://auburn.github.io/FastNoiseLite/
#include <FastNoiseLite.h>

float fnlGetNoise2DWrap(const fnl_state *state, FNLfloat x, FNLfloat y, FNLfloat scale_x, FNLfloat scale_y, FNLfloat w, FNLfloat h);

#endif //P17R_NOISE_H
