#include "chunk.h"
#include "world.h"

tile_t *world_get_tile(world_t *world, size_t x, size_t y)
{
    return chunk_get_tile_absolute(world_get_chunk(world, x, y), x, y);
}

///////////////////////////////////////////////////////////////////////////////

#ifdef TEST
#include <criterion/criterion.h>

Test(world, get_tile)
{
    world_t world = {0};

    cr_assert(world_init(&world, 100, 10));
    cr_assert_eq(world_get_tile(&world, 0, 0),
        chunk_get_tile_relative(REG_AT(chunk_t, &world.chunk_reg, 0), 0, 0));
    cr_assert_eq(world_get_tile(&world, 10, 10),
        chunk_get_tile_relative(REG_AT(chunk_t, &world.chunk_reg, 11), 0, 0));
    cr_assert_eq(world_get_tile(&world, 5, 26),
        chunk_get_tile_relative(REG_AT(chunk_t, &world.chunk_reg, 20), 5, 6));
}

Test(world, get_tile_bad)
{
    world_t world = {0};

    cr_assert(world_init(&world, 100, 10));
    cr_assert_eq(world_get_tile(&world, 100, 0), NULL);
    cr_assert_eq(world_get_tile(&world, 0, 100), NULL);
    cr_assert_eq(world_get_tile(&world, 100, 100), NULL);
    cr_assert_eq(world_get_tile(&world, -1, 12), NULL);
}

#endif