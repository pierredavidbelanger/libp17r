#ifndef P17R_RL_H
#define P17R_RL_H

#include <stdlib.h>

#include "p17r-mem.h"

#include <raylib.h>
#include <raymath.h>

#define RESS_MALLOC(sz) P17R_MALLOC(sz)
#define RESS_CALLOC(n,sz) P17R_CALLOC(n,sz)
#define RESS_REALLOC(ptr,sz) P17R_REALLOC(ptr,sz)
#define RESS_FREE(ptr) P17R_FREE(ptr)
#define RRES_SUPPORT_LOG_INFO 0
#include <rres.h>

#define RAYGUI_MALLOC(sz) P17R_MALLOC(sz)
#define RAYGUI_CALLOC(n,sz) P17R_CALLOC(n,sz)
#define RAYGUI_FREE(p) P17R_FREE(p)
#include <raygui.h>

typedef struct {
    Texture2D texture;
    Rectangle source;
    Vector2 size;
    int rows;
    int cols;
} SpriteSheet;

typedef struct {
    char nameId[128];
    Texture2D texture;
    Rectangle source;
    Vector2 size;
    Vector2 origin;
} Sprite;

typedef struct {
    Texture2D texture;
    size_t spriteCount;
    Sprite *sprites;
} Atlas;

typedef struct {
    char fileName[256];
    rresCentralDir dir;
    size_t atlasCount;
    Atlas *atlases;
} Ress;

float Vector2RotationToLookAt(Vector2 position, Vector2 target);

float RotateAngleTowards(float rotation, float target, float maxRotation);

bool RotationIsFacingLeft(float rotation);

Vector2 Vector2FromScalars(float x, float y);

Vector2 Vector2FromScalar(float s);

Vector2 Vector2Half(void);

Vector2 Vector2Absolute(Vector2 v);

Vector2 Vector2NegateX(Vector2 v);

Vector2 Vector2NegateY(Vector2 v);

Vector2 Vector2CopySign(Vector2 v);

void Vector4Components(Vector4 v, float components[4]);

Vector2 RectangleCorner(Rectangle rectangle);

Vector2 RectangleSize(Rectangle rectangle);

Rectangle RectangleFromCornerAndSize(Vector2 corner, Vector2 size);

Rectangle RectangleFromCornerToCorner(Vector2 corner1, Vector2 corner2);

Rectangle RectangleGrow(Rectangle r, Vector2 delta);

Rectangle RectangleScale(Rectangle r, Vector2 scale);

Rectangle Texture2DRectangle(Texture2D texture);

float GetRandomFloat(float min, float max);

Vector2 GetRandomVector2(Vector2 size);

Vector2 GetRandomVector2InsideRectangle(Rectangle area);

void DrawSprite(Sprite sprite, Vector2 position);

void DrawSpriteEx(Sprite sprite, Vector2 position, float rotation, Vector2 size, Color tint);

Ress LoadRess(const char *fileName);

void UnloadRess(Ress *ress);

Texture2D RessLoadTexture(const Ress *ress, const char *fileName);

Atlas RessLoadAtlas(Ress *ress, const char *fileName);

Sprite AtlasGetSprite(const Atlas *atlas, const char *nameId);

SpriteSheet AtlasGetSpriteSheet(const Atlas *atlas, const char *nameId, int cols, int rows);

Sprite SpriteFromSheet(SpriteSheet sheet, int idx, Vector2 origin);

/**
 * Create bounds Rectangle
 * @param x absolute horizontal position
 * @param y absolute vertical position
 * @param width absolute horizontal size
 * @param height absolute vertical size
 */
Rectangle GuiBounds(float x, float y, float width, float height);

/**
 * Create bounds Rectangle inside parent bounds
 * @param parent the parent bounds
 * @param pad_h absolute horizontal padding
 * @param pad_v absolute vertical padding
 * @param width absolute (or relative as percent if <= 1) horizontal width
 * @param height absolute (or relative as percent if <= 1) vertical height
 */
Rectangle GuiBoundsInside(Rectangle parent, float pad_h, float pad_v, float width, float height);

/**
 * Create bounds Rectangle inside parent bounds and relative to a sibling bounds
 * @param parent the parent bounds
 * @param pad_h absolute horizontal padding
 * @param pad_v absolute vertical padding
 * @param width absolute (or relative as percent if <= 1) horizontal width
 * @param height absolute (or relative as percent if <= 1) vertical height
 * @param sibling the sibling bounds
 * @param vertical stack under the sibling (same horizontal position) if true, or to the right of the sibling (same vertical position) if false
 */
Rectangle GuiBoundsInsideAfter(Rectangle parent, float pad_h, float pad_v, float width, float height, Rectangle sibling, bool vertical);

#endif //P17R_RL_H
