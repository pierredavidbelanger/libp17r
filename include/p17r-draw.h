#ifndef P17R_DRAW_H
#define P17R_DRAW_H

#include <stdbool.h>

#include "p17r-rl.h"
#include "p17r-resources.h"

typedef struct {
    Texture2D texture;
    Rectangle source;
    Vector2 position;
    Vector2 scale;
    Vector2 origin_norm;
    Vector2 origin_offset;
    float rotation;
    bool flip_x;
    bool flip_y;
    bool flip_origin;
    bool flip_rotation;
    Color tint;
} texture_draw_opts_t;

#ifdef __cplusplus
extern "C" {
#endif

texture_draw_opts_t texture_draw_opts_from(const texture_t *texture);
texture_draw_opts_t *texture_draw_opts_translate(texture_draw_opts_t *opts, Vector2 position);
texture_draw_opts_t *texture_draw_opts_scale(texture_draw_opts_t *opts, Vector2 scale);
texture_draw_opts_t *texture_draw_opts_rotate(texture_draw_opts_t *opts, float rotation);
texture_draw_opts_t *texture_draw_opts_origin_norm(texture_draw_opts_t *opts, Vector2 origin_norm);
texture_draw_opts_t *texture_draw_opts_origin_offset(texture_draw_opts_t *opts, Vector2 offset);
texture_draw_opts_t *texture_draw_opts_tint(texture_draw_opts_t *opts, Color tint);
texture_draw_opts_t *texture_draw_opts_alpha(texture_draw_opts_t *opts, float alpha);
texture_draw_opts_t *texture_draw_opts_flip(texture_draw_opts_t *opts, bool flip_x, bool flip_y, bool flip_origin, bool flip_rotation);
void texture_draw(const texture_draw_opts_t *opts);

#ifdef __cplusplus
}
#endif

#endif //P17R_DRAW_H
