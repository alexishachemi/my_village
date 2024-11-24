#pragma once

#include <sys/types.h>
#include "biome.h"
#include "chunk.h"
#include "registry.h"
#include "asset.h"
#include "prop.h"
#include "terrain.h"
#include "texture.h"
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

asset_t *world_new_asset(world_t *world, const char *name,
    texture_t *texture, Rectangle draw_rect);
asset_t *world_get_asset(world_t *world, const char *name);

// Prop

prop_t *world_new_prop(world_t *world, const char *name);
prop_t *world_get_prop(world_t *world, const char *name);

// Terrain

terrain_t *world_new_terrain(world_t *world, const char *name, asset_t *asset);
terrain_t *world_get_terrain(world_t *world, const char *name);

// Biome

biome_t *world_new_biome(world_t *world, const char *name);
biome_t *world_get_biome(world_t *world, const char *name);

// Tile

tile_t *world_get_tile(world_t *world, size_t x, size_t y);

// Chunk

bool world_init_chunks(world_t *world, size_t size, size_t chunk_size);
chunk_t *world_get_chunk(world_t *world, size_t x, size_t y);