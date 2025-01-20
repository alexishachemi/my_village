#pragma once

#include <sys/types.h>
#include "biome.h"
#include "chunk.h"
#include "orientation.h"
#include "registry.h"
#include "asset.h"
#include "prop.h"
#include "terrain.h"
#include "texture.h"
#include "tile.h"
#include "v2.h"

#define WORLD_INIT_REGISTRY(wp, t, sz) (reg_init(&wp->t##s, sizeof(t##_t), sz))
#define WORLD_DEINIT_REGISTRY(wp, t) (reg_deinit(&wp->t##s))

typedef struct {
    size_t size;
    size_t chunk_size;
    reg_t assets;   // asset_t
    reg_t props;    // prop_t
    reg_t terrains; // terrain_t
    reg_t biomes ;  // biome_t
    reg_t chunks;   // chunk_t
    reg_t rooms;    // csp_room_t  
} world_t;

bool world_init(world_t *world, size_t size, size_t chunk_size);
void world_deinit(world_t *world);
void world_print(world_t *world);

bool world_pos_is_valid(world_t *world, v2_t pos);

// Asset

asset_t *world_new_asset(world_t *world, const char *name,
    texture_t *texture, Rectangle draw_rect);
asset_t *world_get_asset(world_t *world, const char *name);

// Prop

prop_t *world_new_prop(world_t *world, const char *name);
prop_t *world_get_prop(world_t *world, const char *name);
void world_remove_prop(world_t *world, v2_t pos);

// Terrain

terrain_t *world_new_terrain(world_t *world, const char *name, asset_t *asset);
terrain_t *world_get_terrain(world_t *world, const char *name);

// Biome

biome_t *world_new_biome(world_t *world, const char *name);
biome_t *world_get_biome(world_t *world, const char *name);

// Tile

tile_t *world_get_tile(world_t *world, v2_t pos);
bool world_place_terrain(world_t *world, v2_t pos, terrain_t *terrain);
bool world_place_prop(world_t *world, prop_t *prop, v2_t pos, orient_t orientation, bool force);
bool world_can_place_prop(world_t *world, prop_t *prop, v2_t pos, orient_t orientation);

// Chunk

bool world_init_chunks(world_t *world, size_t size, size_t chunk_size);
chunk_t *world_get_chunk(world_t *world, v2_t pos);