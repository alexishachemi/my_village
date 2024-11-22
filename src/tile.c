#include "tile.h"

bool tile_init(tile_t *tile)
{
    if (!tile)
        return false;
    tile->terrain_id = -1;
    tile->prop_id = -1;
    tile->biome_id = -1;
    return true;
}

#ifdef TEST
#include <criterion/criterion.h>

Test(tile, init)
{
    tile_t tile = {0};

    cr_assert(tile_init(&tile));
    cr_assert_eq(tile.biome_id, -1);
    cr_assert_eq(tile.prop_id, -1);
    cr_assert_eq(tile.terrain_id, -1);
}

#endif