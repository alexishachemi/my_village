#include <math.h>
#include "rect.h"
#include "utils.h"

bsp_v2_t rect_center(bsp_rect_t r)
{
    return (bsp_v2_t){r.pos.x + r.size.x / 2, r.pos.y + r.size.y / 2};
}

bsp_rect_t rect_merge(bsp_rect_t r1, bsp_rect_t r2)
{
    bsp_v2_t end1, end2;
    int x, y, endx, endy;

    end1 = rect_end(r1);
    end2 = rect_end(r2);
    x = min(r1.pos.x, r2.pos.x);
    y = min(r1.pos.y, r2.pos.y);
    endx = max(end1.x, end2.x);
    endy = max(end1.y, end2.y);
    return rect_new_e(
        (bsp_v2_t){x, y},
        (bsp_v2_t){endx, endy}
    );
}

bsp_rect_t rect_grow(bsp_rect_t r, int n)
{
    return (bsp_rect_t){vec2_sub_i(r.pos, n), vec2_add_i(r.size, n * 2)};
}

int rect_area(bsp_rect_t r)
{
    return r.size.x * r.size.y;
}
