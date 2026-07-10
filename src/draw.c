#include "p17r-draw.h"

texture_draw_opts_t texture_draw_opts_from(const texture_t *texture) {
    return (texture_draw_opts_t){
        .texture = texture->texture,
        .source = texture->source,
        .position = Vector2Zero(),
        .scale = Vector2One(),
        .origin_norm = Vector2Zero(),
        .origin_offset = Vector2Zero(),
        .rotation = 0.0f,
        .flip_x = false,
        .flip_y = false,
        .move_origin = false,
        .tint = WHITE
    };
}

texture_draw_opts_t *texture_draw_opts_translate(texture_draw_opts_t *opts, const Vector2 position) {
    opts->position = Vector2Add(opts->position, position);
    return opts;
}

texture_draw_opts_t *texture_draw_opts_scale(texture_draw_opts_t *opts, const Vector2 scale) {
    opts->scale = Vector2Multiply(opts->scale, scale);
    return opts;
}

texture_draw_opts_t *texture_draw_opts_rotate(texture_draw_opts_t *opts, const float rotation) {
    opts->rotation += rotation;
    return opts;
}

texture_draw_opts_t *texture_draw_opts_origin_norm(texture_draw_opts_t *opts, const Vector2 origin_norm) {
    opts->origin_norm = origin_norm;
    return opts;
}

texture_draw_opts_t *texture_draw_opts_origin_offset(texture_draw_opts_t *opts, const Vector2 offset) {
    opts->origin_offset = Vector2Add(opts->origin_offset, offset);
    return opts;
}

texture_draw_opts_t *texture_draw_opts_tint(texture_draw_opts_t *opts, const Color tint) {
    opts->tint = ColorTint(opts->tint, tint);
    return opts;
}

texture_draw_opts_t *texture_draw_opts_alpha(texture_draw_opts_t *opts, const float alpha) {
    opts->tint = ColorAlpha(opts->tint, alpha);
    return opts;
}

texture_draw_opts_t *texture_draw_opts_flip(texture_draw_opts_t *opts, const bool flip_x, const bool flip_y, const bool move_origin) {
    opts->flip_x = flip_x;
    opts->flip_y = flip_y;
    opts->move_origin = move_origin;
    return opts;
}

void texture_draw(const texture_draw_opts_t *opts) {
    // Get the absolute source size (ensuring it is always positive, even if its pre-flipped)
    Vector2 source_size = Vector2Absolute(RectangleSize(opts->source));
    // Calculate source origin (pre-scaling)
    Vector2 source_origin = Vector2Add(Vector2Multiply(source_size, opts->origin_norm), opts->origin_offset);
    // Apply origin flipping if requested
    if (opts->flip_x && opts->move_origin) source_origin.x = source_size.x - source_origin.x;
    if (opts->flip_y && opts->move_origin) source_origin.y = source_size.y - source_origin.y;
    // Calculate destination size (using source size and scale factor)
    Vector2 destination_size = Vector2Multiply(source_size, opts->scale);
    // Destination Rectangle: in DrawTexturePro, dest.x/y is the pivot position on screen
    Rectangle destination = RectangleFromCornerAndSize(opts->position, destination_size);
    // Calculate destination origin (post-scaling, in Raylib origin is relative to destination)
    Vector2 destination_origin = Vector2Multiply(source_origin, opts->scale);
    // Apply flipping to the source (negative is source rectangle width/height means flip)
    Rectangle source = opts->source;
    if (opts->flip_x) source.width = -source.width;
    if (opts->flip_y) source.height = -source.height;
    DrawTexturePro(opts->texture, source, destination, destination_origin, opts->rotation, opts->tint);
}
