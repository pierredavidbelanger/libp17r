#ifndef P17R_NOISE_H
#define P17R_NOISE_H

typedef struct osn_t osn_t;

osn_t *osn_create(long long seed);

void osn_free(osn_t *osn);

double osn_noise2d_norm(const osn_t *osn, double x, double y, double w, double h, double scale);

double osn_noise2d_wrap_norm(const osn_t *osn, double x, double y, double scale_x, double scale_y, double w, double h, unsigned int octaves, double lacunarity, double gain);

double osn_noise3d_norm(const osn_t *osn, double x, double y, double z, double w, double h, double d, double scale);

#endif //P17R_NOISE_H
