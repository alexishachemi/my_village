#pragma once

#include "raylib.h"
#include "str.h"

typedef struct {
    name_t name;
    path_t path;
    Texture2D rtexture;
} texture_t;

bool texture_init(texture_t *texture, const char *name, const char *path);
bool texture_load(texture_t *texture);
void texture_unload(texture_t *texture);