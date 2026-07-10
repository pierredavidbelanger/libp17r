#include "p17r-rl.h"

#include "p17r-log.h"
#include "p17r-mem.h"

#define RRES_IMPLEMENTATION
#include <rres.h>

#define RRES_RAYLIB_IMPLEMENTATION
// #define RRES_SUPPORT_COMPRESSION_LZ4
// #define RRES_SUPPORT_ENCRYPTION_AES
// #define RRES_SUPPORT_ENCRYPTION_XCHACHA20
#include <rres-raylib.h>

#define RAYGUI_IMPLEMENTATION
// #define RAYGUI_DEBUG_RECS_BOUNDS
// #define RAYGUI_DEBUG_TEXT_BOUNDS
#include <raygui.h>

#include <stdio.h>

float Vector2RotationToLookAt(const Vector2 position, const Vector2 target) {
    return -Vector2LineAngle(position, target) * RAD2DEG - 90.0f;
}

float RotateAngleTowards(const float rotation, const float target, const float maxRotation) {
    float diff = Wrap(target - rotation, -360.0f, 360.0f);
    if (diff < -180.0f) diff += 360.0f;
    if (diff > 180.0f) diff -= 360.0f;
    if (fabsf(diff) <= maxRotation) {
        return target;
    }
    return rotation + (diff > 0.0f ? maxRotation : -maxRotation);
}

bool RotationIsFacingLeft(const float rotation) {
    return -sinf(rotation * DEG2RAD) < 0.0f;
}

Vector2 Vector2FromScalars(const float x, const float y) {
    return (Vector2){.x = x, .y = y};
}

Vector2 Vector2FromScalar(const float s) {
    return Vector2FromScalars(s, s);
}

Vector2 Vector2Half(void) {
    return Vector2FromScalars(0.5f, 0.5f);
}

Vector2 Vector2Absolute(const Vector2 v) {
    return Vector2FromScalars(fabsf(v.x), fabsf(v.y));
}

Vector2 Vector2NegateX(const Vector2 v) {
    return Vector2FromScalars(-v.x, v.y);
}

Vector2 Vector2NegateY(const Vector2 v) {
    return Vector2FromScalars(v.x, -v.y);
}

Vector2 Vector2CopySign(const Vector2 v) {
    return Vector2FromScalars((float) copysign(1.0, v.x), (float) copysign(1.0, v.y));
}

void Vector4Components(const Vector4 v, float components[4]) {
    components[0] = v.x;
    components[1] = v.y;
    components[2] = v.z;
    components[3] = v.w;
}

Vector2 RectangleCorner(const Rectangle rectangle) {
    return Vector2FromScalars(rectangle.x, rectangle.y);
}

Vector2 RectangleSize(const Rectangle rectangle) {
    return Vector2FromScalars(rectangle.width, rectangle.height);
}

Rectangle RectangleFromCornerAndSize(const Vector2 corner, const Vector2 size) {
    return (Rectangle){.x = corner.x, .y = corner.y, .width = size.x, .height = size.y};
}

Rectangle RectangleFromCornerToCorner(const Vector2 corner1, const Vector2 corner2) {
    const Vector2 corner = Vector2Min(corner1, corner2);
    const Vector2 size = Vector2Absolute(Vector2Subtract(corner1, corner2));
    return RectangleFromCornerAndSize(corner, size);
}

Rectangle RectangleGrow(const Rectangle r, const Vector2 delta) {
    Rectangle result = r;
    result.x -= delta.x;
    result.y -= delta.y;
    result.width += delta.x * 2.0f;
    result.height += delta.y * 2.0f;
    return result;
}

Rectangle RectangleScale(const Rectangle r, const Vector2 scale) {
    const Vector2 size = RectangleSize(r);
    const Vector2 delta = Vector2Subtract(Vector2Multiply(size, scale), size);
    return RectangleGrow(r, delta);
}

Rectangle RectanglePad(const Rectangle r, const Vector2 padding) {
    Rectangle result = r;
    result.x += padding.x;
    result.y += padding.y;
    result.width -= padding.x * 2.0f;
    result.height -= padding.y * 2.0f;
    return result;
}

Rectangle Texture2DRectangle(const Texture2D texture) {
    const Vector2 corner = Vector2FromScalar(0.0f);
    const Vector2 size = Vector2FromScalars((float) texture.width, (float) texture.height);
    return RectangleFromCornerAndSize(corner, size);
}

float GetRandomFloat(const float min, const float max) {
    const int intRange = 1000000;
    return Remap((float) GetRandomValue(0, intRange), 0.0f, (float) intRange, min, max);
}

Vector2 GetRandomVector2(const Vector2 size) {
    return Vector2FromScalars(GetRandomFloat(0.0f, size.x), GetRandomFloat(0.0f, size.y));
}

Vector2 GetRandomVector2InsideRectangle(const Rectangle area) {
    return Vector2Add(RectangleCorner(area), GetRandomVector2(RectangleSize(area)));
}

void DrawSprite(const Sprite sprite, const Vector2 position) {
    const Rectangle destination = RectangleFromCornerAndSize(position, sprite.size);
    DrawTexturePro(sprite.texture, sprite.source, destination, sprite.origin, 0.0f, WHITE);
}

void DrawSpriteEx(const Sprite sprite, const Vector2 position,
                  const float rotation, const Vector2 size, const Color tint) {
    const Rectangle destination = RectangleFromCornerAndSize(position, size);
    DrawTexturePro(sprite.texture, sprite.source, destination, sprite.origin, rotation, tint);
}

Ress LoadRess(const char *fileName) {
    Ress ress = {0};
    snprintf(ress.fileName, sizeof(ress.fileName), "%s", fileName);
    ress.dir = rresLoadCentralDirectory(ress.fileName);
    for (unsigned int i = 0; i < ress.dir.count; i++) {
        TRACELOG(LOG_INFO, "RRES: CDIR: File entry %03i: %s", i + 1, ress.dir.entries[i].fileName);
    }
    ress.atlasCount = 0;
    ress.atlases = (Atlas *) P17R_CALLOC(ress.atlasCount, sizeof(Atlas));
    TRACELOG(LOG_INFO, "RRES: Loaded %s", ress.fileName);
    return ress;
}

void UnloadRess(Ress *ress) {
    for (size_t i = 0; i < ress->atlasCount; i++) {
        Atlas *atlas = &ress->atlases[i];
        P17R_FREE(atlas->sprites);
        TRACELOG(LOG_INFO, "RRES: Unloaded %d Sprites for Atlas %d", atlas->spriteCount, i + 1);
        atlas->spriteCount = 0;
        atlas->sprites = NULL;
        UnloadTexture(atlas->texture);
    }
    P17R_FREE(ress->atlases);
    TRACELOG(LOG_INFO, "RRES: Unloaded %d Atlas", ress->atlasCount);
    ress->atlasCount = 0;
    ress->atlases = NULL;
    TRACELOG(LOG_INFO, "RRES: Unloaded %s", ress->fileName);
    rresUnloadCentralDirectory(ress->dir);
}

Texture2D RessLoadTexture(const Ress *ress, const char *fileName) {
    Texture2D texture = {0};
    const int ressId = rresGetResourceId(ress->dir, fileName);
    const rresResourceChunk chunk = rresLoadResourceChunk(ress->fileName, ressId);
    const Image image = LoadImageFromResource(chunk);
    texture = LoadTextureFromImage(image);
    UnloadImage(image);
    return texture;
}

Atlas RessLoadAtlas(Ress *ress, const char *fileName) {
    Atlas atlas = {0};
    const int ressId = rresGetResourceId(ress->dir, fileName);
    rresResourceChunk chunk = rresLoadResourceChunk(ress->fileName, ressId);
    const int result = UnpackResourceChunk(&chunk);
    if (result == 0) {
        unsigned int dataSize = 0;
        char *data = LoadDataFromResource(chunk, &dataSize);
        int dataOffset = 0;
        int dataRead = 0;
        char line[256] = {0};
        while (sscanf(data + dataOffset, "%255[^\n]%n", line, &dataRead) == 1) {
            if (line[0] == 'a') {
                char imagePath[512] = {0};
                // a <imagePath> <width> <height> <spriteCount> <isFont> <fontSize>
                sscanf(line, "a %s %*d %*d %*d %*d %*d", imagePath);
                atlas.texture = RessLoadTexture(ress, imagePath);
                atlas.spriteCount = 0;
                atlas.sprites = (Sprite *) P17R_CALLOC(atlas.spriteCount, sizeof(Sprite));
            } else if (line[0] == 's') {
                char nameId[128] = {0};
                int originX = 0, originY = 0;
                int positionX = 0, positionY = 0;
                int sourceWidth = 0, sourceHeight = 0;
                // s <nameId> <tag> <originX> <originY> <positionX> <positionY> <sourceWidth> <sourceHeight> <padding> <trimmed> <trimRecX> <trimRecY> <trimRecWidth> <trimRecHeight> <colliderType> <colliderX> <colliderY> <colliderSizeX> <colliderSizeY>
                sscanf(line, "s %s %*s %d %d %d %d %d %d %*d %*d %*d %*d %*d %*d %*d %*d %*d %*d %*d", nameId, &originX, &originY, &positionX, &positionY, &sourceWidth, &sourceHeight);
                const Vector2 position = Vector2FromScalars(positionX, positionY);
                const Vector2 size = Vector2FromScalars(sourceWidth, sourceHeight);
                const Vector2 origin = Vector2FromScalars(originX, originY);
                Sprite sprite = {0};
                snprintf(sprite.nameId, sizeof(sprite.nameId), "%s", nameId);
                sprite.texture = atlas.texture;
                sprite.source = RectangleFromCornerAndSize(position, size);
                sprite.size = size;
                sprite.origin = origin;
                atlas.spriteCount++;
                atlas.sprites = (Sprite *) P17R_REALLOC(atlas.sprites, atlas.spriteCount * sizeof(Sprite));
                atlas.sprites[atlas.spriteCount - 1] = sprite;
            }
            dataOffset += dataRead;
            if (data[dataOffset] == '\n') {
                dataOffset++;
            }
        }
        P17R_FREE(data);
    }
    ress->atlasCount++;
    ress->atlases = (Atlas *) P17R_REALLOC(ress->atlases, ress->atlasCount * sizeof(Atlas));
    ress->atlases[ress->atlasCount - 1] = atlas;
    TRACELOG(LOG_INFO, "RRES: Atlas loaded successfully (Texture [ID %d], %d Sprites)", atlas.texture.id, atlas.spriteCount);
    return atlas;
}

Sprite AtlasGetSprite(const Atlas *atlas, const char *nameId) {
    Sprite sprite = {0};
    for (size_t i = 0; i < atlas->spriteCount; i++) {
        if (strcmp(atlas->sprites[i].nameId, nameId) == 0) {
            TRACELOG(LOG_INFO, "ATLAS: Sprite %s found", nameId);
            sprite = atlas->sprites[i];
            break;
        }
    }
    if (sprite.texture.id == 0) {
        TRACELOG(LOG_INFO, "ATLAS: Sprite %s NOT found", nameId);
    }
    return sprite;
}

SpriteSheet AtlasGetSpriteSheet(const Atlas *atlas, const char *nameId, const int cols, const int rows) {
    SpriteSheet sheet = {0};
    const Sprite sprite = AtlasGetSprite(atlas, nameId);
    sheet.texture = sprite.texture;
    sheet.source = sprite.source;
    sheet.size = Vector2Divide(sprite.size, Vector2FromScalars(rows, cols));
    sheet.rows = rows;
    sheet.cols = cols;
    return sheet;
}

Sprite SpriteFromSheet(const SpriteSheet sheet, const int idx, const Vector2 origin) {
    Sprite sprite = {0};
    const Vector2 index = Vector2FromScalars(idx % sheet.cols, idx / sheet.rows);
    const Vector2 corner = Vector2Add(RectangleCorner(sheet.source), Vector2Multiply(index, sheet.size));
    const Rectangle source = RectangleFromCornerAndSize(corner, sheet.size);
    sprite.texture = sheet.texture;
    sprite.source = source;
    sprite.size = sheet.size;
    sprite.origin = Vector2Multiply(origin, sheet.size);
    return sprite;
}

Rectangle GuiBounds(const float x, const float y, const float width, const float height) {
    return (Rectangle){
        .x = x,
        .y = y,
        .width = width,
        .height = height,
    };
}

Rectangle GuiBoundsInside(const Rectangle parent, const float pad_h, const float pad_v, const float width, const float height) {
    return (Rectangle){
        .x = parent.x + pad_h,
        .y = parent.y + pad_v,
        .width = width > 1 ? width : parent.width * width - pad_h - pad_h,
        .height = height > 1 ? height : parent.height * height - pad_v - pad_v,
    };
}

Rectangle GuiBoundsInsideAfter(const Rectangle parent, const float pad_h, const float pad_v, const float width, const float height, const Rectangle sibling, const bool vertical) {
    return (Rectangle){
        .x = !vertical ? sibling.x + sibling.width + pad_h : sibling.x,
        .y = vertical ? sibling.y + sibling.height + pad_v : sibling.y,
        .width = width > 1 ? width : parent.width * width - pad_h - pad_h,
        .height = height > 1 ? height : parent.height * height - pad_v - pad_v,
    };
}
