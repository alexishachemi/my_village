#include <stdio.h>
#include "asset.h"
#include "biome.h"
#include "csp.h"
#include "prop.h"
#include "registry.h"
#include "world.h"
#include "chunk.h"

bool world_init(world_t *world, size_t size, size_t chunk_size)
{
    if (!world || size == 0)
        return false;
    world->size = size;
    return WORLD_INIT_REGISTRY(world, asset, ASSET_REGISTRY_BASE_SIZE)
        && WORLD_INIT_REGISTRY(world, prop, PROP_REGISTRY_BASE_SIZE)
        && WORLD_INIT_REGISTRY(world, terrain, TERRAIN_REGISTRY_BASE_SIZE)
        && WORLD_INIT_REGISTRY(world, biome, BIOME_REGISTRY_BASE_SIZE)
        && WORLD_INIT_REGISTRY(world, chunk, CHUNK_REGISTRY_BASE_SIZE)
        && reg_init(&world->rooms, sizeof(csp_room_t), CSP_ROOM_REGISTRY_BASE_SIZE)
        && world_init_chunks(world, size, chunk_size);
}

void world_deinit(world_t *world)
{
    if (!world)
        return;
    WORLD_DEINIT_REGISTRY(world, asset);
    reg_map(&world->props, (reg_callback_t)prop_deinit);
    WORLD_DEINIT_REGISTRY(world, prop);
    WORLD_DEINIT_REGISTRY(world, terrain);
    reg_map(&world->biomes, (reg_callback_t)biome_deinit);
    WORLD_DEINIT_REGISTRY(world, biome);
    reg_map(&world->chunks, (reg_callback_t)chunk_deinit);
    WORLD_DEINIT_REGISTRY(world, chunk);
    reg_map(&world->rooms, (reg_callback_t)csp_room_deinit);
    WORLD_DEINIT_REGISTRY(world, room);
}
