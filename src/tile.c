#include "tile.h"
#include "orientation.h"

bool tile_init(tile_t *tile)
{
    if (!tile)
        return false;
    tile->terrain = NULL;
    tile->prop = NULL;
    tile->biome = NULL;
    tile->prop_orient = ORIENT_DOWN;
    return true;
}

#ifdef TEST
#include <criterion/criterion.h>

Test(tile, init)
{
    tile_t tile = {0};

    cr_assert(tile_init(&tile));
    cr_assert_null(tile.terrain);
    cr_assert_null(tile.prop);
    cr_assert_null(tile.biome);
    cr_assert_eq(tile.prop_orient, ORIENT_DOWN);
}

#endif