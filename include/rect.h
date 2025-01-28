#pragma once

#include <stdbool.h>
#include "raylib.h"

typedef struct {
    int x;
    int y;
    int w;
    int h;
} rect_t;

bool rect_contains_rect(rect_t r1, rect_t r2);
Rectangle rect_to_raylib(rect_t rect);