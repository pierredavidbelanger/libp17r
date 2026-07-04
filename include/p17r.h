#ifndef P17R_H
#define P17R_H

#include <stdio.h>
#include <stdlib.h>

#if IS_DEBUG_BUILD
#ifndef P17R_MALLOC
#define P17R_MALLOC(sz)      malloc(sz);printf("P17R_MALLOC(%d)\n",sz)
#endif
#ifndef P17R_CALLOC
#define P17R_CALLOC(n,sz)    calloc(n,sz);printf("P17R_CALLOC(%d,%d)\n",n,sz)
#endif
#ifndef P17R_REALLOC
#define P17R_REALLOC(ptr,sz) realloc(ptr,sz);printf("P17R_REALLOC(*,%d)\n",sz)
#endif
#ifndef P17R_FREE
#define P17R_FREE(ptr)       free(ptr);printf("P17R_FREE(*)\n")
#endif
#else
#ifndef P17R_MALLOC
#define P17R_MALLOC(sz)      malloc(sz)
#endif
#ifndef P17R_CALLOC
#define P17R_CALLOC(n,sz)    calloc(n,sz)
#endif
#ifndef P17R_REALLOC
#define P17R_REALLOC(ptr,sz) realloc(ptr,sz)
#endif
#ifndef P17R_FREE
#define P17R_FREE(ptr)       free(ptr)
#endif
#endif

typedef struct osn_t osn_t;
osn_t *osn_create(long long seed);
void osn_free(osn_t *osn);
double osn_noise2d_norm(const osn_t *osn, double x, double y, double w, double h, double scale);
double osn_noise3d_norm(const osn_t *osn, double x, double y, double z, double w, double h, double d, double scale);

#define RL_MALLOC(sz)        P17R_MALLOC(sz)
#define RL_CALLOC(n,sz)      P17R_CALLOC(n,sz)
#define RL_REALLOC(ptr,sz)   P17R_REALLOC(ptr,sz)
#define RL_FREE(ptr)         P17R_FREE(ptr)
#include <raylib.h>
#include <raymath.h>

#define RESS_MALLOC(sz)      P17R_MALLOC(sz)
#define RESS_CALLOC(n,sz)    P17R_CALLOC(n,sz)
#define RESS_REALLOC(ptr,sz) P17R_REALLOC(ptr,sz)
#define RESS_FREE(ptr)       P17R_FREE(ptr)
#include <rres.h>

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
Vector2 Vector2FromScalars(float x, float y);
Vector2 Vector2FromScalar(float s);
Vector2 Vector2Half();
Vector2 Vector2Absolute(Vector2 v);
Vector2 RectangleCorner(Rectangle rectangle);
Vector2 RectangleSize(Rectangle rectangle);
Rectangle RectangleFromCornerAndSize(Vector2 corner, Vector2 size);
Rectangle RectangleFromCornerToCorner(Vector2 corner1, Vector2 corner2);
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

#endif // P17R_H
