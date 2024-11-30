#include "chunk.h"
#include "registry.h"
#include "v2.h"
#include "world.h"
#include <stdio.h>

static bool init_chunk_line(world_t *world, size_t size,
    size_t chunk_size, size_t y, size_t size_rest)
{
    chunk_t *chunk = NULL;
    size_t max_chunk_size = chunk_size > size ? size : chunk_size;

    for (size_t x = 0; x < size - size_rest; x += max_chunk_size) {
        chunk = reg_new_elem(&world->chunk_reg);
        if (!chunk)
            return false;
        if (!chunk_init(chunk, &(chunk_bounds_t){
                x, y, x + max_chunk_size - 1, y + max_chunk_size - 1
            }))
            return false;
    }
    if (size_rest == 0)
        return true;
    chunk = reg_new_elem(&world->chunk_reg);
    return chunk && chunk_init(chunk, &(chunk_bounds_t){size - size_rest,
        y, size - 1, y + max_chunk_size - 1});
}

bool world_init_chunks(world_t *world, size_t size, size_t chunk_size)
{
    size_t size_rest = size % chunk_size;

    if (!world || size == 0 || chunk_size == 0)
        return false;
    for (size_t y = 0; y < size - size_rest; y += chunk_size) {
        if (!init_chunk_line(world, size, chunk_size, y, size_rest))
            return false;
    }
    if (size_rest == 0)
        return true;
    return init_chunk_line(world, size, size_rest, size - size_rest, 0);
}

static bool is_in_bounds(const chunk_bounds_t *b, v2_t pos)
{
    return b->from_x <= (unsigned int)pos.x && (unsigned int)pos.x <= b->to_x
        && b->from_y <= (unsigned int)pos.y && (unsigned int)pos.y <= b->to_y;
}

chunk_t *world_get_chunk(world_t *world, v2_t pos)
{
    chunk_t *chunk = NULL;

    if (!world || (unsigned int)pos.x >= world->size || (unsigned int)pos.y >= world->size)
        return NULL;
    for (size_t i = 0; i < world->chunk_reg.last_free_index; i++) {
        chunk = REG_AT(chunk_t, &world->chunk_reg, i);
        if (chunk && is_in_bounds(&chunk->bounds, pos))
            return chunk;
    }
    return NULL;
}

///////////////////////////////////////////////////////////////////////////////

#ifdef TEST
#include <criterion/criterion.h>

Test(world, chunk_bad_init)
{
    world_t world = {0};

    cr_assert_not(world_init(&world, 0, 0));
    cr_assert_not(world_init(&world, 10, 0));
    cr_assert_not(world_init(&world, 0, 10));
}

static int count_tiles(world_t *world)
{
    int tiles = 0;
    chunk_t *chunk = NULL;

    for (unsigned int i = 0; i < REG_SIZE(world->chunk_reg); i++) {
        chunk = REG_AT(chunk_t, &world->chunk_reg, i);
        tiles += chunk->tiles.size;
    }
    return tiles;
}

Test(world, chunk_init_chunk_fragmentation)
{
    unsigned int max_size = 20;
    unsigned int max_chunk_sizes = 20;
    world_t world = {0};

    for (unsigned int size = 2; size <= max_size; size++) {
        for (unsigned int chunk_size = 2; chunk_size <= max_chunk_sizes; chunk_size++) {
            cr_assert(world_init(&world, size, chunk_size),
                "Failed to init a %dx%d world with %dx%d chunks", size, size, chunk_size, chunk_size);
            cr_assert_eq(count_tiles(&world), size * size);
            world_deinit(&world);
        }
    }
}

Test(world, chunk_init_too_big)
{
    world_t world = {0};

    cr_assert(world_init(&world, 50, 100));
    cr_assert_eq(REG_AT(chunk_t, &world.chunk_reg, 0)->bounds.to_x, 49);
    world_deinit(&world);
}

Test(world, get_chunk_single_chunk)
{
    world_t world = {0};

    cr_assert(world_init(&world, 10, 10));
    cr_assert_eq(world_get_chunk(&world, (v2_t){0, 0}),
        REG_AT(chunk_t, &world.chunk_reg, 0));
    cr_assert_eq(world_get_chunk(&world, (v2_t){9, 9}),
        REG_AT(chunk_t, &world.chunk_reg, 0));
    cr_assert_eq(world_get_chunk(&world, (v2_t){10, 10}), NULL);
    world_deinit(&world);
}

Test(world, get_chunk_multiple_chunks)
{
    world_t world = {0};

    cr_assert(world_init(&world, 10, 5));
    cr_assert_eq(world_get_chunk(&world, (v2_t){0, 0}),
        REG_AT(chunk_t, &world.chunk_reg, 0));
    cr_assert_eq(world_get_chunk(&world, (v2_t){4, 4}),
        REG_AT(chunk_t, &world.chunk_reg, 0));
    cr_assert_eq(world_get_chunk(&world, (v2_t){5, 0}),
        REG_AT(chunk_t, &world.chunk_reg, 1));
    cr_assert_eq(world_get_chunk(&world, (v2_t){9, 9}),
        REG_AT(chunk_t, &world.chunk_reg, 3));
    cr_assert_eq(world_get_chunk(&world, (v2_t){10, 10}), NULL);
    world_deinit(&world);
}

Test(world, get_chunk_lots_of_chunks)
{
    world_t world = {0};

    cr_assert(world_init(&world, 100, 10));
    cr_assert_eq(world_get_chunk(&world, (v2_t){0, 0}),
        REG_AT(chunk_t, &world.chunk_reg, 0));
    cr_assert_eq(world_get_chunk(&world, (v2_t){9, 9}),
        REG_AT(chunk_t, &world.chunk_reg, 0));
    cr_assert_eq(world_get_chunk(&world, (v2_t){10, 10}),
        REG_AT(chunk_t, &world.chunk_reg, 11));
    cr_assert_eq(world_get_chunk(&world, (v2_t){19, 19}),
        REG_AT(chunk_t, &world.chunk_reg, 11));
    cr_assert_eq(world_get_chunk(&world, (v2_t){20, 20}),
        REG_AT(chunk_t, &world.chunk_reg, 22));
    cr_assert_eq(world_get_chunk(&world, (v2_t){99, 99}),
        REG_AT(chunk_t, &world.chunk_reg, 99));
    cr_assert_eq(world_get_chunk(&world, (v2_t){100, 100}), NULL);
    world_deinit(&world);
}

#endif