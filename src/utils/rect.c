#include "rect.h"

bool rect_contains_rect(rect_t r1, rect_t r2)
{
    int bl1[2] = {r1.x + r1.w, r1.y + r1.h};
    int bl2[2] = {r2.x + r2.w, r2.y + r2.h};

    return r1.x <= r2.x && r1.y <= r2.y
        && bl1[0] >= bl2[0] && bl1[1] >= bl2[1];
}

Rectangle rect_to_raylib(rect_t rect)
{
    return (Rectangle){rect.x, rect.y, rect.w, rect.h};
}
