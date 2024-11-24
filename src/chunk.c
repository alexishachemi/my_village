#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include "chunk.h"
#include "cvec.h"
#include "tile.h"

bool chunk_bounds_size(const chunk_bounds_t *bounds, int *x, int *y)
{
    if (!bounds || !x || !y)
        return false;
    *x = bounds->to_x - bounds->from_x + 1;
    *y = bounds->to_y - bounds->from_y + 1;
    return true;
}

int chunk_bounds_area(const chunk_bounds_t *bounds)
{
    int x = 0;
    int y = 0;

    if (!chunk_bounds_size(bounds, &x, &y))
        return 0;
    return x * y;
}

static bool bounds_are_valid(const chunk_bounds_t *bounds)
{
    return bounds
        && bounds->from_x < bounds->to_x
        && bounds->from_y < bounds->to_y
        && bounds->to_x - bounds->from_x > 0
        && bounds->to_y - bounds->from_y > 0;
}

bool chunk_init(chunk_t *chunk, const chunk_bounds_t *bounds)
{
    if (!chunk || !bounds_are_valid(bounds))
        return false;
    memcpy(&chunk->bounds, bounds, sizeof(chunk_bounds_t));
    chunk->tiles = vec_create(sizeof(tile_t));
    vec_resize(&chunk->tiles, chunk_bounds_area(&chunk->bounds));
    for (size_t i = 0; i < vec_size(&chunk->tiles); i++) {
        if (!tile_init(vec_fast_at(&chunk->tiles, i)))
            return false;
    }
    return true;
}

void chunk_deinit(chunk_t *chunk)
{
    if (!chunk)
        return;
    vec_free_data(&chunk->tiles);
}

static bool is_in_bounds(const chunk_bounds_t *bounds, size_t x, size_t y)
{
    return x >= bounds->from_x && x <= bounds->to_x
        && y >= bounds->from_y && y <= bounds->to_y;
}

tile_t *chunk_get_tile_absolute(chunk_t *chunk, size_t x, size_t y)
{
    size_t idx = 0;
    size_t line_size = 0;
    size_t relative_x = 0;
    size_t relative_y = 0;

    if (!chunk || !is_in_bounds(&chunk->bounds, x, y))
        return NULL;
    relative_x = x - chunk->bounds.from_x;
    relative_y = y - chunk->bounds.from_y;
    line_size = chunk->bounds.to_x - chunk->bounds.from_x + 1;
    idx = relative_y * line_size + relative_x;
    return vec_fast_at(&chunk->tiles, idx);
}

tile_t *chunk_get_tile_relative(chunk_t *chunk, size_t x, size_t y)
{
    size_t idx = 0;
    size_t line_size = 0;

    if (!chunk 
        || x > chunk->bounds.to_x - chunk->bounds.from_x
        || y > chunk->bounds.to_y - chunk->bounds.from_y)
        return NULL;
    line_size = chunk->bounds.to_x - chunk->bounds.from_x + 1;
    idx = y * line_size + x;
    return vec_fast_at(&chunk->tiles, idx);
}

void chunk_print(chunk_t *chunk)
{
    chunk_bounds_t b = {0};

    if (!chunk) {
        printf("(null)");
        return;
    }
    b = chunk->bounds;
    printf("---\n");
    printf("X range:\t%ld\t%ld\n", b.from_x, b.to_x);
    printf("Y range:\t%ld\t%ld\n", b.from_y, b.to_y);
    printf("Tiles: %ld\n", chunk->tiles.size);
    printf("---\n");
}

///////////////////////////////////////////////////////////////////////////////

#ifdef TEST
#include <criterion/criterion.h>

Test(chunk, init)
{
    chunk_t chunk = {0};

    cr_assert(chunk_init(&chunk, &(chunk_bounds_t){0, 0, 10, 10}));
    cr_assert_eq(chunk.bounds.from_x, 0);
    cr_assert_eq(chunk.bounds.from_y, 0);
    cr_assert_eq(chunk.bounds.to_x, 10);
    cr_assert_eq(chunk.bounds.to_y, 10);
    cr_assert_eq(vec_size(&chunk.tiles), 121);
    for (size_t i = 0; i < vec_size(&chunk.tiles); i++) {
        cr_assert_null(VEC_FAST_AT(tile_t, &chunk.tiles, i).terrain);
        cr_assert_null(VEC_FAST_AT(tile_t, &chunk.tiles, i).prop);
        cr_assert_null(VEC_FAST_AT(tile_t, &chunk.tiles, i).biome);
    }
}

Test(chunk, bad_init)
{
    chunk_t chunk = {0};

    cr_assert_not(chunk_init(&chunk, &(chunk_bounds_t){0, 0, 0, 0}));
    cr_assert_not(chunk_init(&chunk, &(chunk_bounds_t){0, 0, 10, 0}));
    cr_assert_not(chunk_init(&chunk, &(chunk_bounds_t){0, 0, 0, 10}));
}

Test(chunk, get_tile_absolute)
{
    chunk_t chunk = {0};
    tile_t *tile = NULL;

    cr_assert(chunk_init(&chunk, &(chunk_bounds_t){10, 10, 19, 19}));
    cr_assert_eq(chunk_get_tile_absolute(
        vec_fast_at(&chunk.tiles, 13), 12, 11), tile);
    cr_assert_eq(chunk_get_tile_absolute(
        &chunk, 10, 10), vec_fast_at(&chunk.tiles, 0));
    cr_assert_eq(chunk_get_tile_absolute(
        &chunk, 19, 19), vec_fast_at(&chunk.tiles, 99));
    cr_assert_eq(chunk_get_tile_absolute(
        &chunk, 19, 10), vec_fast_at(&chunk.tiles, 9));
}

Test(chunk, get_tile_absolute_bad)
{
    chunk_t chunk = {0};

    cr_assert(chunk_init(&chunk, &(chunk_bounds_t){10, 10, 19, 19}));
    cr_assert_eq(chunk_get_tile_absolute(&chunk, 9, 10), NULL);
    cr_assert_eq(chunk_get_tile_absolute(&chunk, 10, 9), NULL);
    cr_assert_eq(chunk_get_tile_absolute(&chunk, 20, 10), NULL);
    cr_assert_eq(chunk_get_tile_absolute(&chunk, 10, 20), NULL);
}

Test(chunk, get_tile_relative)
{
    chunk_t chunk = {0};

    cr_assert(chunk_init(&chunk, &(chunk_bounds_t){10, 10, 19, 19}));
    cr_assert_eq(chunk_get_tile_relative(&chunk, 0, 0),
        vec_fast_at(&chunk.tiles, 0));
    cr_assert_eq(chunk_get_tile_relative(&chunk, 9, 9),
        vec_fast_at(&chunk.tiles, 99));
    cr_assert_eq(chunk_get_tile_relative(&chunk, 9, 0),
        vec_fast_at(&chunk.tiles, 9));
}

Test(chunk, get_tile_relative_bad)
{
    chunk_t chunk = {0};

    cr_assert(chunk_init(&chunk, &(chunk_bounds_t){10, 10, 19, 19}));
    cr_assert_eq(chunk_get_tile_relative(&chunk, 11, 11), NULL);
    cr_assert_eq(chunk_get_tile_relative(&chunk, 10, 11), NULL);
    cr_assert_eq(chunk_get_tile_relative(&chunk, 11, 10), NULL);
    cr_assert_eq(chunk_get_tile_relative(&chunk, 10, 10), NULL);
}

#endif