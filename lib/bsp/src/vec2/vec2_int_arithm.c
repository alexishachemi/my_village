#include "vec2.h"

bsp_v2_t vec2_add_i(bsp_v2_t a, int n)
{
    return (bsp_v2_t){a.x + n, a.y + n};
}

bsp_v2_t vec2_sub_i(bsp_v2_t a, int n)
{
    return (bsp_v2_t){a.x - n, a.y - n};
}

bsp_v2_t vec2_mul_i(bsp_v2_t a, int n)
{
    return (bsp_v2_t){a.x * n, a.y * n};
}

bsp_v2_t vec2_div_i(bsp_v2_t a, int n)
{
    bsp_v2_t c = {0, 0};

    if (n != 0) {
        c.x = a.x / n;
        c.y = a.y / n;
    }
    return c;
}
