#include "v2.h"

v2_t v2_orient(v2_t v, orient_t orientation)
{
    switch (orientation) {
        case ORIENT_DOWN:
            return v;
        case ORIENT_UP:
            return (v2_t){-v.x, -v.y};
        case ORIENT_LEFT:
            return (v2_t){-v.y, v.x};
        case ORIENT_RIGHT:
            return (v2_t){v.y, -v.x};
        default:
            return v;
    }
}

Vector2 v2_to_raylib(v2_t v)
{
    return (Vector2){v.x, v.y};
}

////////////////////////////////////////////////////////////////////////////////

#ifdef TEST
#include <criterion/criterion.h>

Test(v2, orient)
{
    v2_t v = {1, 2};

    cr_assert_eq(v2_orient(v, ORIENT_UP).x, -1);
    cr_assert_eq(v2_orient(v, ORIENT_UP).y, -2);

    cr_assert_eq(v2_orient(v, ORIENT_DOWN).x, 1);
    cr_assert_eq(v2_orient(v, ORIENT_DOWN).y, 2);

    cr_assert_eq(v2_orient(v, ORIENT_LEFT).x, -2);
    cr_assert_eq(v2_orient(v, ORIENT_LEFT).y, 1);

    cr_assert_eq(v2_orient(v, ORIENT_RIGHT).x, 2);
    cr_assert_eq(v2_orient(v, ORIENT_RIGHT).y, -1);
}

Test(v2, to_raylib)
{
    v2_t v = {1, 2};
    Vector2 v2 = v2_to_raylib(v);

    cr_assert_eq(v2.x, 1);
    cr_assert_eq(v2.y, 2);
}

#endif