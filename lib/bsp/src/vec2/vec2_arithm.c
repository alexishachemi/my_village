#include "vec2.h"

bsp_v2_t vec2_add(bsp_v2_t a, bsp_v2_t b)
{
    return (bsp_v2_t){a.x + b.x, a.y + b.y};
}

bsp_v2_t vec2_sub(bsp_v2_t a, bsp_v2_t b)
{
    return (bsp_v2_t){a.x - b.x, a.y - b.y};
}

bsp_v2_t vec2_mul(bsp_v2_t a, bsp_v2_t b)
{
    return (bsp_v2_t){a.x * b.x, a.y * b.y};
}

bsp_v2_t vec2_div(bsp_v2_t a, bsp_v2_t b)
{
    bsp_v2_t c = {0, 0};

    if (b.x != 0)
        c.x = a.x / b.x;
    if (b.y != 0)
        c.y = a.y / b.y;
    return c;
}
