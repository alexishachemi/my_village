#include "world.h"

bool world_init(world_t *world)
{
    if (!world)
        return false;
    return W_INIT_REG(world, asset, ASSET_REGISTRY_BASE_SIZE)
        && W_INIT_REG(world, prop, PROP_REGISTRY_BASE_SIZE);
}

void world_deinit(world_t *world)
{
    if (!world)
        return;
    W_DEINIT_REG(world, asset);
    W_DEINIT_REG(world, prop);
}