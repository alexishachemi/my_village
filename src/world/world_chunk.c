#include "chunk.h"
#include "registry.h"
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

static bool is_in_bounds(const chunk_bounds_t *b, size_t x, size_t y)
{
    return b->from_x <= x && x <= b->to_x
        && b->from_y <= y && y <= b->to_y;
}

chunk_t *world_get_chunk(world_t *world, size_t x, size_t y)
{
    chunk_t *chunk = NULL;

    if (!world || x >= world->size || y >= world->size)
        return NULL;
    for (size_t i = 0; i < world->chunk_reg.last_free_index; i++) {
        chunk = REG_AT(chunk_t, &world->chunk_reg, i);
        if (chunk && is_in_bounds(&chunk->bounds, x, y))
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

Test(world, chunk_init_too_big)
{
    world_t world = {0};

    cr_assert(world_init(&world, 50, 100));
    cr_assert_eq(REG_AT(chunk_t, &world.chunk_reg, 0)->bounds.to_x, 49);
}

Test(world, get_chunk_single_chunk)
{
    world_t world = {0};

    cr_assert(world_init(&world, 10, 10));
    cr_assert_eq(world_get_chunk(&world, 0, 0),
        REG_AT(chunk_t, &world.chunk_reg, 0));
    cr_assert_eq(world_get_chunk(&world, 9, 9),
        REG_AT(chunk_t, &world.chunk_reg, 0));
    cr_assert_eq(world_get_chunk(&world, 10, 10), NULL);
}

Test(world, get_chunk_multiple_chunks)
{
    world_t world = {0};

    cr_assert(world_init(&world, 10, 5));
    cr_assert_eq(world_get_chunk(&world, 0, 0),
        REG_AT(chunk_t, &world.chunk_reg, 0));
    cr_assert_eq(world_get_chunk(&world, 4, 4),
        REG_AT(chunk_t, &world.chunk_reg, 0));
    cr_assert_eq(world_get_chunk(&world, 5, 0),
        REG_AT(chunk_t, &world.chunk_reg, 1));
    cr_assert_eq(world_get_chunk(&world, 9, 9),
        REG_AT(chunk_t, &world.chunk_reg, 3));
    cr_assert_eq(world_get_chunk(&world, 10, 10), NULL);
}

Test(world, get_chunk_lots_of_chunks)
{
    world_t world = {0};

    cr_assert(world_init(&world, 100, 10));
    cr_assert_eq(world_get_chunk(&world, 0, 0),
        REG_AT(chunk_t, &world.chunk_reg, 0));
    cr_assert_eq(world_get_chunk(&world, 9, 9),
        REG_AT(chunk_t, &world.chunk_reg, 0));
    cr_assert_eq(world_get_chunk(&world, 10, 10),
        REG_AT(chunk_t, &world.chunk_reg, 11));
    cr_assert_eq(world_get_chunk(&world, 19, 19),
        REG_AT(chunk_t, &world.chunk_reg, 11));
    cr_assert_eq(world_get_chunk(&world, 20, 20),
        REG_AT(chunk_t, &world.chunk_reg, 22));
    cr_assert_eq(world_get_chunk(&world, 99, 99),
        REG_AT(chunk_t, &world.chunk_reg, 99));
    cr_assert_eq(world_get_chunk(&world, 100, 100), NULL);
}

#endif