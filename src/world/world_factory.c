#include "biome.h"
#include "world.h"
#include "chunk.h"

bool world_init(world_t *world, size_t size, size_t chunk_size)
{
    if (!world || size == 0 || chunk_size == 0)
        return false;
    return W_INIT_REG(world, asset, ASSET_REGISTRY_BASE_SIZE)
        && W_INIT_REG(world, prop, PROP_REGISTRY_BASE_SIZE)
        && W_INIT_REG(world, terrain, TERRAIN_REGISTRY_BASE_SIZE)
        && W_INIT_REG(world, biome, BIOME_REGISTRY_BASE_SIZE)
        && W_INIT_REG(world, chunk, CHUNK_REGISTRY_BASE_SIZE)
        && world_init_chunks(world, size, chunk_size);
}

void world_deinit(world_t *world)
{
    if (!world)
        return;
    W_DEINIT_REG(world, asset);
    W_DEINIT_REG(world, prop);
    W_DEINIT_REG(world, terrain);
    W_DEINIT_REG(world, biome);
    reg_map(&world->chunk_reg, (void (*)(void *))chunk_deinit);
    W_DEINIT_REG(world, chunk);
}