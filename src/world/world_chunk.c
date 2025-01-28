#include <stdio.h>
#include "chunk.h"
#include "registry.h"
#include "v2.h"
#include "world.h"

static bool init_chunk_line(world_t *world, size_t size, size_t chunk_size, size_t y)
{
    chunk_t *chunk = NULL;
    chunk_bounds_t bounds = {0};

    for (size_t x = 0; x < size; x += chunk_size) {
        chunk = reg_new_elem(&world->chunks);
        if (!chunk)
            return false;
        bounds = (chunk_bounds_t){x, y, x + chunk_size - 1, y + chunk_size - 1};
        if (!chunk_init(chunk, &bounds))
            return false;
    }
    return true;
}

static size_t compute_chunk_size(size_t size)
{
    size_t *csize = NULL;
    reg_t chunk_sizes = {0};

    if (!reg_init(&chunk_sizes, sizeof(size_t), size / 2))
        return 0;
    for (int i = size - 1; i > 0; i--) {
        if (size % i != 0)
            continue;
        csize = reg_new_elem(&chunk_sizes);
        if (!csize)
            return 0;
        *csize = i;
    }
    if (REG_SIZE(chunk_sizes)) {
        size = *REG_AT(size_t, &chunk_sizes, 0);
    } else {
        size = *REG_AT(size_t, &chunk_sizes, REG_SIZE(chunk_sizes) / 2 - 1);
    }
    reg_deinit(&chunk_sizes);
    return size;
}

bool world_init_chunks(world_t *world, size_t size, size_t chunk_size)
{
    if (chunk_size == 0)
        chunk_size = compute_chunk_size(size);
    world->chunk_size = chunk_size;
    if (!world || size == 0 || chunk_size == 0)
        return false;
    if (size % chunk_size != 0) {
        dprintf(2, "Invalid world size (chunk_size must be a multiple of size)\n");
        return false;
    }
    for (size_t y = 0; y < size; y += chunk_size) {
        if (!init_chunk_line(world, size, chunk_size, y))
            return false;
    }
    return true;
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
    for (size_t i = 0; i < world->chunks.last_free_index; i++) {
        chunk = REG_AT(chunk_t, &world->chunks, i);
        if (chunk && is_in_bounds(&chunk->bounds, pos))
            return chunk;
    }
    return NULL;
}

///////////////////////////////////////////////////////////////////////////////

#ifdef TEST
#include <criterion/criterion.h>
#include <criterion/redirect.h>

Test(world, chunk_bad_init)
{
    world_t world = {0};

    cr_assert_not(world_init(&world, 0, 0));
    cr_assert_not(world_init(&world, 0, 10));
}

static int count_tiles(world_t *world)
{
    int tiles = 0;
    chunk_t *chunk = NULL;

    for (unsigned int i = 0; i < REG_SIZE(world->chunks); i++) {
        chunk = REG_AT(chunk_t, &world->chunks, i);
        tiles += chunk->tiles.size;
    }
    return tiles;
}

Test(world, chunk_init_chunk_fragmentation, .init=cr_redirect_stderr)
{
    unsigned int max_size = 20;
    unsigned int max_chunk_sizes = 20;
    size_t tiles = 0;
    world_t world = {0};
    bool success = false;

    for (unsigned int size = 2; size <= max_size; size++) {
        for (unsigned int chunk_size = 2; chunk_size <= max_chunk_sizes; chunk_size++) {
            success = world_init(&world, size, chunk_size);
            if (size % chunk_size != 0) {
                cr_assert_not(success);
                continue;
            }
            cr_assert(success, "Failed to init a (size: %d, chunk_size: %d)", size, chunk_size);
            tiles = count_tiles(&world);
            cr_assert_eq(tiles, size * size,
                "Wrong tile count. Expected %d, got %ld (size: %d, chunk_size: %d)",
                size * size, tiles, size, chunk_size);
            world_deinit(&world);
        }
    }
}

Test(world, chunk_init_too_big, .init=cr_redirect_stderr)
{
    world_t world = {0};

    cr_assert_not(world_init(&world, 50, 100));
}

Test(world, get_chunk_single_chunk)
{
    world_t world = {0};

    cr_assert(world_init(&world, 10, 10));
    cr_assert_eq(world_get_chunk(&world, (v2_t){0, 0}),
        REG_AT(chunk_t, &world.chunks, 0));
    cr_assert_eq(world_get_chunk(&world, (v2_t){9, 9}),
        REG_AT(chunk_t, &world.chunks, 0));
    cr_assert_eq(world_get_chunk(&world, (v2_t){10, 10}), NULL);
    world_deinit(&world);
}

Test(world, get_chunk_multiple_chunks)
{
    world_t world = {0};

    cr_assert(world_init(&world, 10, 5));
    cr_assert_eq(world_get_chunk(&world, (v2_t){0, 0}),
        REG_AT(chunk_t, &world.chunks, 0));
    cr_assert_eq(world_get_chunk(&world, (v2_t){4, 4}),
        REG_AT(chunk_t, &world.chunks, 0));
    cr_assert_eq(world_get_chunk(&world, (v2_t){5, 0}),
        REG_AT(chunk_t, &world.chunks, 1));
    cr_assert_eq(world_get_chunk(&world, (v2_t){9, 9}),
        REG_AT(chunk_t, &world.chunks, 3));
    cr_assert_eq(world_get_chunk(&world, (v2_t){10, 10}), NULL);
    world_deinit(&world);
}

Test(world, get_chunk_lots_of_chunks)
{
    world_t world = {0};

    cr_assert(world_init(&world, 100, 10));
    cr_assert_eq(world_get_chunk(&world, (v2_t){0, 0}),
        REG_AT(chunk_t, &world.chunks, 0));
    cr_assert_eq(world_get_chunk(&world, (v2_t){9, 9}),
        REG_AT(chunk_t, &world.chunks, 0));
    cr_assert_eq(world_get_chunk(&world, (v2_t){10, 10}),
        REG_AT(chunk_t, &world.chunks, 11));
    cr_assert_eq(world_get_chunk(&world, (v2_t){19, 19}),
        REG_AT(chunk_t, &world.chunks, 11));
    cr_assert_eq(world_get_chunk(&world, (v2_t){20, 20}),
        REG_AT(chunk_t, &world.chunks, 22));
    cr_assert_eq(world_get_chunk(&world, (v2_t){99, 99}),
        REG_AT(chunk_t, &world.chunks, 99));
    cr_assert_eq(world_get_chunk(&world, (v2_t){100, 100}), NULL);
    world_deinit(&world);
}

#endif