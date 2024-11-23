#pragma once

#include <sys/types.h>
#include "biome.h"
#include "chunk.h"
#include "registry.h"
#include "asset.h"
#include "prop.h"
#include "terrain.h"
#include "tile.h"

#define W_INIT_REG(wp, t, s) (reg_init(&wp->t##_reg, sizeof(t##_t), s))
#define W_DEINIT_REG(wp, t) (reg_deinit(&wp->t##_reg))

typedef struct {
    size_t size;
    reg_t asset_reg;    // asset_t
    reg_t prop_reg;     // prop_t
    reg_t terrain_reg;  // terrain_t
    reg_t biome_reg ;   // biome_t
    reg_t chunk_reg;    // chunk_t
} world_t;

bool world_init(world_t *world, size_t size, size_t chunk_size);
void world_deinit(world_t *world);

// Asset

ssize_t world_new_asset(world_t *world, const char *name, const char *path);
ssize_t world_get_asset(world_t *world, const char *name);
char *world_get_asset_path(world_t *world, const char *name);
asset_t *world_get_asset_ptr(world_t *world, size_t asset_id);

// Prop

ssize_t world_new_prop(world_t *world, const char *name);
ssize_t world_get_prop(world_t *world, const char *name);
prop_t *world_get_prop_ptr(world_t *world, size_t prop_id);

// Terrain

ssize_t world_new_terrain(world_t *world, const char *name, size_t asset_id);
ssize_t world_get_terrain(world_t *world, const char *name);
terrain_t *world_get_terrain_ptr(world_t *world, size_t terrain_id);

// Biome

ssize_t world_add_biome(world_t *world, const char *name);
ssize_t world_get_biome(world_t *world, const char *name);
biome_t *world_get_biome_ptr(world_t *world, size_t biome_id);

// Tile

tile_t *world_get_tile(world_t *world, size_t x, size_t y);

// Chunk

bool world_init_chunks(world_t *world, size_t size, size_t chunk_size);
chunk_t *world_get_chunk(world_t *world, size_t x, size_t y);