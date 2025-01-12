#include "rect.h"

bsp_rect_t rect_new_e(bsp_v2_t start, bsp_v2_t end)
{
    return (bsp_rect_t){start, vec2_sub(end, start)};
}

bsp_v2_t rect_end(bsp_rect_t r)
{
    return vec2_add(r.pos, r.size);
}
