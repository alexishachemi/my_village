#pragma once

#include "cvec.h"
#include "rect.h"
#include "tile.h"

typedef struct {
    rect_t rect;
    vec_t tiles; // tile_t
} chunk_t;

bool chunk_init(
    chunk_t *chunk, size_t from_x, size_t from_y, size_t to_x, size_t to_y
);
bool chunk_init_with_rect(chunk_t *chunk, rect_t rect);

/// @brief Gets a tile from a chunk. The coordinates are relative to
/// the rectangle of the chunk. (rect.x, rect.y) is the top-left.
/// @param chunk The chunk.
/// @param x The x coordinate of the tile.
/// @param y The y coordinate of the tile.
/// @return The tile at the given coordinates. NULL on error.
tile_t *chunk_get_tile_absolute(chunk_t *chunk, size_t x, size_t y);

/// @brief Gets a tile from a chunk. The coordinates are absolute.
/// (0, 0) is the top-left of the chunk.
/// @param chunk The chunk.
/// @param x The x coordinate of the tile.
/// @param y The y coordinate of the tile.
/// @return The tile at the given coordinates. NULL on error.
tile_t *chunk_get_tile_relative(chunk_t *chunk, size_t x, size_t y);