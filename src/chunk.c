#include "chunk.h"
#include "cvec.h"
#include "rect.h"
#include "tile.h"
#include <stdbool.h>

bool chunk_init(
    chunk_t *chunk, size_t from_x, size_t from_y, size_t to_x, size_t to_y
)
{
    if (!chunk || from_x >= to_x || from_y >= to_y)
        return false;
    return chunk_init_with_rect(
        chunk, rect_from_bounds(from_x, from_y, to_x + 1, to_y + 1));
}

bool chunk_init_with_rect(chunk_t *chunk, rect_t rect)
{
    if (!chunk || rect.w == 0 || rect.h == 0)
        return false;
    chunk->rect = rect;
    chunk->tiles = vec_create(sizeof(tile_t));
    vec_resize(&chunk->tiles, rect_area(&rect));
    for (size_t i = 0; i < vec_size(&chunk->tiles); i++) {
        if (!tile_init(vec_fast_at(&chunk->tiles, i)))
            return false;
    }
    return true;
}

static bool is_in_bounds(const rect_t *rect, size_t x, size_t y)
{
    return x >= rect->x && x < rect->x + rect->w
        && y >= rect->y && y < rect->y + rect->h;
}

tile_t *chunk_get_tile_absolute(chunk_t *chunk, size_t x, size_t y)
{
    size_t idx = 0;

    if (!chunk || !is_in_bounds(&chunk->rect, x, y))
        return NULL;
    idx = (y - chunk->rect.y) * chunk->rect.w + (x - chunk->rect.x);
    return vec_fast_at(&chunk->tiles, idx);
}

tile_t *chunk_get_tile_relative(chunk_t *chunk, size_t x, size_t y)
{
    size_t idx = 0;

    if (!chunk || x >= chunk->rect.w || y >= chunk->rect.h)
        return NULL;
    idx = y * chunk->rect.w + x;
    return vec_fast_at(&chunk->tiles, idx);
}

///////////////////////////////////////////////////////////////////////////////

#ifdef TEST
#include <criterion/criterion.h>

Test(chunk, init)
{
    chunk_t chunk = {0};

    cr_assert(chunk_init(&chunk, 0, 0, 10, 10));
    cr_assert_eq(chunk.rect.x, 0);
    cr_assert_eq(chunk.rect.y, 0);
    cr_assert_eq(chunk.rect.w, 11);
    cr_assert_eq(chunk.rect.h, 11);
    cr_assert_eq(vec_size(&chunk.tiles), 121);
    for (size_t i = 0; i < vec_size(&chunk.tiles); i++) {
        cr_assert_eq(VEC_FAST_AT(tile_t, &chunk.tiles, i).terrain_id, -1);
        cr_assert_eq(VEC_FAST_AT(tile_t, &chunk.tiles, i).prop_id, -1);
        cr_assert_eq(VEC_FAST_AT(tile_t, &chunk.tiles, i).biome_id, -1);
    }
}

Test(chunk, bad_init)
{
    chunk_t chunk = {0};

    cr_assert_not(chunk_init(&chunk, 0, 0, 0, 0));
    cr_assert_not(chunk_init(&chunk, 0, 0, 10, 0));
    cr_assert_not(chunk_init(&chunk, 0, 0, 0, 10));
}

Test(chunk, get_tile_absolute)
{
    chunk_t chunk = {0};
    tile_t *tile = NULL;

    cr_assert(chunk_init(&chunk, 10, 10, 19, 19));
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

    cr_assert(chunk_init(&chunk, 10, 10, 19, 19));
    cr_assert_eq(chunk_get_tile_absolute(
        &chunk, 9, 9), NULL);
    cr_assert_eq(chunk_get_tile_absolute(
        &chunk, 20, 20), NULL);
}

Test(chunk, get_tile_relative)
{
    chunk_t chunk = {0};

    cr_assert(chunk_init(&chunk, 10, 10, 19, 19));
    cr_assert_eq(chunk_get_tile_relative(
        &chunk, 0, 0), vec_fast_at(&chunk.tiles, 0));
    cr_assert_eq(chunk_get_tile_relative(
        &chunk, 9, 9), vec_fast_at(&chunk.tiles, 99));
    cr_assert_eq(chunk_get_tile_relative(
        &chunk, 5, 7), vec_fast_at(&chunk.tiles, 75));
}

Test(chunk, get_tile_relative_bad)
{
    chunk_t chunk = {0};

    cr_assert(chunk_init(&chunk, 10, 10, 19, 19));
    cr_assert_eq(chunk_get_tile_relative(
        &chunk, 10, 10), NULL);
    cr_assert_eq(chunk_get_tile_relative(
        &chunk, 20, 20), NULL);
}

#endif