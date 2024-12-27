#include <stdio.h>
#include "asset.h"
#include "biome.h"
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
        && world_init_chunks(world, size, chunk_size);
}

void world_deinit(world_t *world)
{
    if (!world)
        return;
    WORLD_DEINIT_REGISTRY(world, asset);
    reg_map(&world->prop_reg, (reg_callback_t)prop_deinit);
    WORLD_DEINIT_REGISTRY(world, prop);
    WORLD_DEINIT_REGISTRY(world, terrain);
    reg_map(&world->biome_reg, (reg_callback_t)biome_deinit);
    WORLD_DEINIT_REGISTRY(world, biome);
    reg_map(&world->chunk_reg, (reg_callback_t)chunk_deinit);
    WORLD_DEINIT_REGISTRY(world, chunk);
}

static void print_assets(reg_t *reg)
{
    asset_t *curr = NULL;

    printf("\n-- Assets --\n");
    for (unsigned int i = 0; i < reg->last_free_index; i++) {
        curr = REG_AT(asset_t, reg, i);
        printf(
            "\t- [%s] texture: %s, [%d, %d, %d, %d]\n",
            curr->name, curr->texture->name, (int)curr->draw_rect.x,
            (int)curr->draw_rect.y, (int)curr->draw_rect.width, (int)curr->draw_rect.height
        );
    }
}

static void print_props(reg_t *reg)
{
    prop_t *curr = NULL;

    printf("\n-- Props --\n");
    for (unsigned int i = 0; i < reg->last_free_index; i++) {
        curr = REG_AT(prop_t, reg, i);
        printf(
            "\t- [%s]\n", curr->name
        );
    }
}

void world_print(world_t *world)
{
    if (!world)
        return;
    printf("-- World --\n\t- size: %ld\n\t- chunk_size: %ld\n", world->size, world->chunk_size);
    print_assets(&world->asset_reg);
    print_props(&world->prop_reg);
}
