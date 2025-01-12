#include <stdio.h>
#include <math.h>
#include "vec2.h"
#include "utils.h"

float vec2_dist(bsp_v2_t a, bsp_v2_t b)
{
    return sqrt(pow(b.x - a.x, 2) + pow(b.y - a.y, 2));
}

static int int_move_towards(int from, int to, int delta)
{
    if (from < to)
        return min(from + delta, to);
    if (from > to)
        return max(from - delta, to);
    return from;
}

bsp_v2_t vec2_move_towards(bsp_v2_t from, bsp_v2_t to, int delta)
{
    return (bsp_v2_t){
        int_move_towards(from.x, to.x, delta),
        int_move_towards(from.y, to.y, delta)
    };
}

void vec2_print(bsp_v2_t vec)
{
    printf("(%d, %d)", vec.x, vec.y);
}
