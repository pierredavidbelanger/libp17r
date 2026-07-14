#ifndef P17R_RESOURCES_H
#define P17R_RESOURCES_H

#include <stdlib.h>

#include "p17r-rl.h"

#ifndef P17R_RESOURCES_TEXTURES_MAX
    #define P17R_RESOURCES_TEXTURES_MAX 512
#endif

#ifndef P17R_RESOURCES_TEXTURE_ATLAS_ITEM_NAME_MAX
    #define P17R_RESOURCES_TEXTURE_ATLAS_ITEM_NAME_MAX 256
#endif

#ifndef P17R_RESOURCES_TEXTURE_ATLAS_ITEMS_MAX
    #define P17R_RESOURCES_TEXTURE_ATLAS_ITEMS_MAX 1024
#endif

typedef struct {
    Texture2D textures[P17R_RESOURCES_TEXTURES_MAX];
    size_t texture_count;
} resources_t;

typedef struct {
    Vector2 corner;
    Vector2 size;
} texture_rectangle_t;

typedef struct {
    Texture2D texture;
    union {
        Rectangle source;
        texture_rectangle_t rectangle;
    };
} texture_t;

typedef struct {
    char name[P17R_RESOURCES_TEXTURE_ATLAS_ITEM_NAME_MAX];
    texture_t texture;
} texture_atlas_item_t;

typedef struct {
    texture_atlas_item_t items[P17R_RESOURCES_TEXTURE_ATLAS_ITEMS_MAX];
    size_t item_count;
} texture_atlas_t;

#ifdef __cplusplus
extern "C" {
#endif

void resources_unload_all(resources_t *resources);
texture_t resources_load_texture(resources_t *resources, const char *filename);
size_t texture_atlas(const texture_t *texture, Vector2 size, texture_atlas_t *out);
size_t resources_load_texture_atlas(resources_t *resources, const char *filepath, Vector2 size, texture_atlas_t *out);
size_t resources_load_texture_atlas_json(resources_t *resources, const char *filepath, texture_atlas_t *out);
size_t texture_atlas_get(const texture_atlas_t *atlas, const char *name, texture_t *out);
size_t texture_atlas_find(const texture_atlas_t *atlas, const char *name_format, size_t max, texture_t *out);

#ifdef __cplusplus
}
#endif

#endif //P17R_RESOURCES_H
