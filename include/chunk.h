#pragma once

#include "cvec.h"
#include "tile.h"

#define CHUNK_REGISTRY_BASE_SIZE 1

typedef struct {
    size_t from_x;
    size_t from_y;
    size_t to_x;
    size_t to_y;
} chunk_bounds_t;

typedef struct {
    chunk_bounds_t bounds;
    vec_t tiles; // tile_t
} chunk_t;

/// Bounds

bool chunk_bounds_size(const chunk_bounds_t *bounds, int *x, int *y);
int chunk_bounds_area(const chunk_bounds_t *bounds);

/// Chunk

/// @brief Initializes a chunk with that store tiles within the given range.
/// @param chunk The chunk to initialize.
/// @param bounds The bounds defining which x and y
/// positions this chunk represents. (inclusive)
/// @return true on success, false otherwise.
bool chunk_init(chunk_t *chunk, const chunk_bounds_t *bounds);

void chunk_deinit(chunk_t *chunk);

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

void chunk_print(chunk_t *chunk);