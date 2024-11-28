#pragma once

#include "raylib.h"
#include "orientation.h"

#define V2_EQ(v1, v2) (v1.x == v2.x && v1.y == v2.y)
#define V2_ADD(v1, v2) ((v2_t){v1.x + v2.x, v1.y + v2.y})

typedef struct {
    int x;
    int y;
} v2_t;

v2_t v2_orient(v2_t v, orient_t orientation);
Vector2 v2_to_raylib(v2_t v);