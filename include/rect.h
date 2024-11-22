#pragma once

#include "stddef.h"

typedef struct {
    size_t x;
    size_t y;
    size_t w;
    size_t h;
} rect_t;

rect_t rect_from_bounds(size_t x1, size_t y1, size_t x2, size_t y2);
size_t rect_area(const rect_t *rect);