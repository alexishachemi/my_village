#include "rect.h"

rect_t rect_from_bounds(size_t x1, size_t y1, size_t x2, size_t y2)
{
    return (rect_t){
        .x = x1,
        .y = y1,
        .w = x2 - x1,
        .h = y2 - y1
    };
}

size_t rect_area(const rect_t *rect)
{
    return rect ? rect->w * rect->h : 0;
}
