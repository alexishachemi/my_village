#include <math.h>
#include "v2.h"

v2_t v2_rotate(v2_t v, float angle)
{
    float x = v.x * cos(angle) - v.y * sin(angle);
    float y = v.x * sin(angle) + v.y * cos(angle);

    return (v2_t){x, y};
}

Vector2 v2_to_raylib(v2_t v)
{
    return (Vector2){v.x, v.y};
}