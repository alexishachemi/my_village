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

#endif