#pragma once

#include <stdbool.h>
#include "raylib.h"
#include "str.h"
#include "texture.h"

#define ASSET_REGISTRY_BASE_SIZE 128

typedef struct {
    name_t name;
    texture_t *texture;
    Rectangle draw_rect;
} asset_t;

bool asset_init(asset_t *asset, const char *name,
    texture_t *texture, Rectangle draw_rect);