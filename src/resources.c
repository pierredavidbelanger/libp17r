#include "p17r-resources.h"

#include "p17r-log.h"
#include "p17r-json.h"

#include <stdio.h>
#include <string.h>
#include <stdbool.h>

void resources_unload_all(resources_t *resources) {
    if (resources != NULL) {
        for (size_t i = 0; i < resources->texture_count; i++) {
            UnloadTexture(resources->textures[i]);
        }
        resources->texture_count = 0;
    }
}

texture_t resources_load_texture(resources_t *resources, const char *filename) {
    texture_t result = {0};

    if (resources->texture_count + 1 > P17R_RESOURCES_TEXTURES_MAX) {
        TRACELOG(LOG_ERROR, "resources_load_texture: Max (%d > %d) texture capacity reached", resources->texture_count + 1, P17R_RESOURCES_TEXTURES_MAX);
        return result;
    }

    const Texture2D texture = LoadTexture(filename);
    if (texture.id <= 0) {
        TRACELOG(LOG_ERROR, "resources_load_texture: cannot load texture %s", filename);
        return result;
    }
    resources->textures[resources->texture_count] = texture;
    resources->texture_count++;

    result.texture = texture;
    result.corner = Vector2Zero();
    result.size = Vector2FromScalars((float) texture.width, (float) texture.height);

    return result;
}

size_t texture_atlas(const texture_t *texture, const Vector2 size, texture_atlas_t *out) {
    if (texture == NULL || out == NULL) {
        return 0;
    }

    const Vector2 grid = Vector2Divide(texture->size, size);
    const size_t item_count = (size_t) grid.x * (size_t) grid.y;

    if (item_count > P17R_RESOURCES_TEXTURE_ATLAS_ITEMS_MAX) {
        TRACELOG(LOG_ERROR, "resources_load_texture_atlas: Max (%d > %d) texture atlas item capacity", item_count, P17R_RESOURCES_TEXTURE_ATLAS_ITEMS_MAX);
        return 0;
    }

    for (size_t y = 0; y < (size_t) grid.y; ++y) {
        for (size_t x = 0; x < (size_t) grid.x; ++x) {
            texture_atlas_item_t *item = &out->items[out->item_count++];

            snprintf(item->name, sizeof(item->name), "%zu,%zu", x, y);
            item->texture = *texture;
            item->texture.corner = Vector2Multiply(Vector2FromScalars((float) x, (float) y), size);
            item->texture.size = size;
        }
    }

    return out->item_count;
}

size_t resources_load_texture_atlas(resources_t *resources, const char *filepath, const Vector2 size, texture_atlas_t *out) {
    if (out == NULL) {
        return 0;
    }

    const texture_t texture = resources_load_texture(resources, filepath);
    if (texture.texture.id <= 0) {
        TRACELOG(LOG_ERROR, "resources_load_texture_atlas: invalid texture %s", filepath);
        return 0;
    }

    return texture_atlas(&texture, size, out);
}

size_t resources_load_texture_atlas_json(resources_t *resources, const char *filepath, texture_atlas_t *out) {
    if (out == NULL) {
        return 0;
    }

    // data
    int data_size = {0};
    unsigned char *data = LoadFileData(filepath, &data_size);
    if (data == NULL) {
        TRACELOG(LOG_ERROR, "resources_load_texture_atlas_json: NULL data in %s", filepath);
        return 0;
    }

    // root
    cJSON *root = cJSON_ParseWithLength((const char *) data, data_size);
    if (root == NULL) {
        const char *error_ptr = cJSON_GetErrorPtr();
        if (error_ptr != NULL) {
            TRACELOG(LOG_ERROR, "resources_load_texture_atlas_json: %s", error_ptr);
        } else {
            TRACELOG(LOG_ERROR, "resources_load_texture_atlas_json: error");
        }
        goto end;
    }
    if (!cJSON_IsObject(root)) {
        TRACELOG(LOG_ERROR, "resources_load_texture_atlas_json: expected . as object");
        goto end;
    }

    // meta image
    const cJSON *meta = cJSON_GetObjectItemCaseSensitive(root, "meta");
    if (!cJSON_IsObject(meta)) {
        TRACELOG(LOG_ERROR, "resources_load_texture_atlas_json: expected .meta as object");
        goto end;
    }
    const cJSON *image = cJSON_GetObjectItemCaseSensitive(meta, "image");
    if (!cJSON_IsString(image) && image->valuestring != NULL) {
        TRACELOG(LOG_ERROR, "resources_load_texture_atlas_json: expected string at .meta.image");
        goto end;
    }
    const char *dirpath = GetDirectoryPath(filepath);
    char image_filepath[P17R_RESOURCES_TEXTURE_ATLAS_ITEM_NAME_MAX] = {0};
    snprintf(image_filepath, sizeof(image_filepath), "%s/%s", dirpath, image->valuestring);
    const texture_t image_texture = resources_load_texture(resources, image_filepath);
    if (image_texture.texture.id <= 0) {
        TRACELOG(LOG_ERROR, "resources_load_texture_atlas_json: invalid texture %s", image_filepath);
        goto end;
    }

    // frames
    const cJSON *frames = cJSON_GetObjectItemCaseSensitive(root, "frames");
    if (!cJSON_IsArray(frames)) {
        TRACELOG(LOG_ERROR, "resources_load_texture_atlas_json: expected array at .frames");
        goto end;
    }
    const int item_count = cJSON_GetArraySize(frames);

    if (item_count > P17R_RESOURCES_TEXTURE_ATLAS_ITEMS_MAX) {
        TRACELOG(LOG_ERROR, "resources_load_texture_atlas_json: Max (%d > %d) sprite item capacity reached", item_count, P17R_RESOURCES_TEXTURE_ATLAS_ITEMS_MAX);
        goto end;
    }

    // frame
    const cJSON *frame = NULL;
    cJSON_ArrayForEach(frame, frames) {
        const cJSON *frame_filename = cJSON_GetObjectItemCaseSensitive(frame, "filename");
        if (!cJSON_IsString(frame_filename) || frame_filename->valuestring == NULL) {
            TRACELOG(LOG_ERROR, "resources_load_texture_atlas_json: expected string at .meta.frames[].filename");
            goto end;
        }
        const cJSON *frame_frame = cJSON_GetObjectItemCaseSensitive(frame, "frame");
        if (!cJSON_IsObject(frame_frame)) {
            TRACELOG(LOG_ERROR, "resources_load_texture_atlas_json: expected object at .meta.frames[].frame");
            goto end;
        }
        const cJSON *frame_frame_x = cJSON_GetObjectItemCaseSensitive(frame_frame, "x");
        if (!cJSON_IsNumber(frame_frame_x)) {
            TRACELOG(LOG_ERROR, "resources_load_texture_atlas_json: expected number at .meta.frames[].frame.x");
            goto end;
        }
        const cJSON *frame_frame_y = cJSON_GetObjectItemCaseSensitive(frame_frame, "y");
        if (!cJSON_IsNumber(frame_frame_y)) {
            TRACELOG(LOG_ERROR, "resources_load_texture_atlas_json: expected number at .meta.frames[].frame.y");
            goto end;
        }
        const cJSON *frame_frame_w = cJSON_GetObjectItemCaseSensitive(frame_frame, "w");
        if (!cJSON_IsNumber(frame_frame_w)) {
            TRACELOG(LOG_ERROR, "resources_load_texture_atlas_json: expected number at .meta.frames[].frame.w");
            goto end;
        }
        const cJSON *frame_frame_h = cJSON_GetObjectItemCaseSensitive(frame_frame, "h");
        if (!cJSON_IsNumber(frame_frame_h)) {
            TRACELOG(LOG_ERROR, "resources_load_texture_atlas_json: expected number at .meta.frames[].frame.h");
            goto end;
        }

        texture_atlas_item_t *item = &out->items[out->item_count++];

        snprintf(item->name, sizeof(item->name), "%s", frame_filename->valuestring);
        item->texture = image_texture;
        item->texture.corner.x = (float) frame_frame_x->valuedouble;
        item->texture.corner.y = (float) frame_frame_y->valuedouble;
        item->texture.size.x = (float) frame_frame_w->valuedouble;
        item->texture.size.y = (float) frame_frame_h->valuedouble;
    }

end:
    cJSON_Delete(root);
    UnloadFileData(data);
    return out->item_count;
}

static size_t texture_atlas_get_internal(const texture_atlas_t *atlas, const char *name, texture_t *out, const bool warn) {
    if (out == NULL) {
        return 0;
    }

    for (size_t i = 0; i < atlas->item_count; i++) {
        const texture_atlas_item_t *item = &atlas->items[i];
        if (strncmp(item->name, name, sizeof(item->name)) == 0) {
            *out = item->texture;
            return 1;
        }
    }

    if (warn) TRACELOG(LOG_WARNING, "texture_atlas_get: %s is not found", name);
    return 0;
}


size_t texture_atlas_get(const texture_atlas_t *atlas, const char *name, texture_t *out) {
    return texture_atlas_get_internal(atlas, name, out, true);
}

size_t texture_atlas_find(const texture_atlas_t *atlas, const char *name_format, const size_t max, texture_t *out) {
    if (out == NULL) {
        return 0;
    }

    size_t total = 0;
    char name[P17R_RESOURCES_TEXTURE_ATLAS_ITEM_NAME_MAX] = {0};
    texture_t texture = {0};

    snprintf(name, sizeof(name), name_format, 0);
    if (texture_atlas_get_internal(atlas, name, &texture, false) > 0) {
        out[total++] = texture;
    }

    for (size_t i = 1; i < max; i++) {
        snprintf(name, sizeof(name), name_format, i);
        if (texture_atlas_get_internal(atlas, name, &texture, false) > 0) {
            out[total++] = texture;
        } else {
            break;
        }
    }

    if (total == 0) {
        TRACELOG(LOG_WARNING, "texture_atlas_find: name %s found no texture", name_format);
    }

    return total;
}
