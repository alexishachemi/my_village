#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include "chunk.h"
#include "cvec.h"
#include "tile.h"
#include "v2.h"

bool chunk_bounds_size(const chunk_bounds_t *bounds, v2_t *pos_buf)
{
    if (!bounds || !pos_buf)
        return false;
    pos_buf->x = bounds->to_x - bounds->from_x;
    pos_buf->y = bounds->to_y - bounds->from_y;
    // if (pos_buf->x == 0 && pos_buf->y == 0)
    //     return true;
    pos_buf->x++;
    pos_buf->y++;
    return true;
}

int chunk_bounds_area(const chunk_bounds_t *bounds)
{
    v2_t pos = {0};

    if (!chunk_bounds_size(bounds, &pos))
        return 0;
    return pos.x * pos.y;
}

static bool bounds_are_valid(const chunk_bounds_t *bounds)
{
    return bounds
        && bounds->from_x <= bounds->to_x
        && bounds->from_y <= bounds->to_y;
}

bool chunk_init(chunk_t *chunk, const chunk_bounds_t *bounds)
{
    int area = 0;

    if (!chunk || !bounds_are_valid(bounds))
        return false;
    memcpy(&chunk->bounds, bounds, sizeof(chunk_bounds_t));
    chunk->tiles = vec_create(sizeof(tile_t));
    area = chunk_bounds_area(bounds);
    if (area <= 0)
        return false;
    vec_resize(&chunk->tiles, area);
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

static bool is_in_bounds(const chunk_bounds_t *bounds, v2_t pos)
{
    return (unsigned int)pos.x >= bounds->from_x && (unsigned int)pos.x <= bounds->to_x
        && (unsigned int)pos.y >= bounds->from_y && (unsigned int)pos.y <= bounds->to_y;
}

tile_t *chunk_get_tile_absolute(chunk_t *chunk, v2_t pos)
{
    size_t idx = 0;
    size_t line_size = 0;
    size_t relative_x = 0;
    size_t relative_y = 0;

    if (!chunk || !is_in_bounds(&chunk->bounds, pos))
        return NULL;
    relative_x = pos.x - chunk->bounds.from_x;
    relative_y = pos.y - chunk->bounds.from_y;
    line_size = chunk->bounds.to_x - chunk->bounds.from_x + 1;
    idx = relative_y * line_size + relative_x;
    return vec_fast_at(&chunk->tiles, idx);
}

tile_t *chunk_get_tile_relative(chunk_t *chunk, v2_t pos)
{
    size_t idx = 0;
    size_t line_size = 0;

    if (!chunk 
        || (unsigned int)pos.x > chunk->bounds.to_x - chunk->bounds.from_x
        || (unsigned int)pos.y > chunk->bounds.to_y - chunk->bounds.from_y)
        return NULL;
    line_size = chunk->bounds.to_x - chunk->bounds.from_x + 1;
    idx = pos.y * line_size + pos.x;
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

Test(chunk, get_tile_absolute)
{
    chunk_t chunk = {0};
    tile_t *tile = NULL;

    cr_assert(chunk_init(&chunk, &(chunk_bounds_t){10, 10, 19, 19}));
    cr_assert_eq(chunk_get_tile_absolute(
        vec_fast_at(&chunk.tiles, 13), (v2_t){12, 11}), tile);
    cr_assert_eq(chunk_get_tile_absolute(
        &chunk, (v2_t){10, 10}), vec_fast_at(&chunk.tiles, 0));
    cr_assert_eq(chunk_get_tile_absolute(
        &chunk, (v2_t){19, 19}), vec_fast_at(&chunk.tiles, 99));
    cr_assert_eq(chunk_get_tile_absolute(
        &chunk, (v2_t){19, 10}), vec_fast_at(&chunk.tiles, 9));
}

Test(chunk, get_tile_absolute_bad)
{
    chunk_t chunk = {0};

    cr_assert(chunk_init(&chunk, &(chunk_bounds_t){10, 10, 19, 19}));
    cr_assert_eq(chunk_get_tile_absolute(&chunk, (v2_t){9, 10}), NULL);
    cr_assert_eq(chunk_get_tile_absolute(&chunk, (v2_t){10, 9}), NULL);
    cr_assert_eq(chunk_get_tile_absolute(&chunk, (v2_t){20, 10}), NULL);
    cr_assert_eq(chunk_get_tile_absolute(&chunk, (v2_t){10, 20}), NULL);
}

Test(chunk, get_tile_relative)
{
    chunk_t chunk = {0};

    cr_assert(chunk_init(&chunk, &(chunk_bounds_t){10, 10, 19, 19}));
    cr_assert_eq(chunk_get_tile_relative(&chunk, (v2_t){0, 0}),
        vec_fast_at(&chunk.tiles, 0));
    cr_assert_eq(chunk_get_tile_relative(&chunk, (v2_t){9, 9}),
        vec_fast_at(&chunk.tiles, 99));
    cr_assert_eq(chunk_get_tile_relative(&chunk, (v2_t){9, 0}),
        vec_fast_at(&chunk.tiles, 9));
}

Test(chunk, get_tile_relative_bad)
{
    chunk_t chunk = {0};

    cr_assert(chunk_init(&chunk, &(chunk_bounds_t){10, 10, 19, 19}));
    cr_assert_eq(chunk_get_tile_relative(&chunk, (v2_t){11, 11}), NULL);
    cr_assert_eq(chunk_get_tile_relative(&chunk, (v2_t){10, 11}), NULL);
    cr_assert_eq(chunk_get_tile_relative(&chunk, (v2_t){11, 10}), NULL);
    cr_assert_eq(chunk_get_tile_relative(&chunk, (v2_t){10, 10}), NULL);
}

#endif