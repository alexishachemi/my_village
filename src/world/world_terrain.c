#include "registry.h"
#include "terrain.h"
#include "world.h"

ssize_t world_new_terrain(world_t *world, const char *name, size_t asset_id)
{
    terrain_t *terrain = NULL;

    if (!world || !name || !world_get_asset_ptr(world, asset_id)
        || world_get_terrain(world, name) != -1)
        return -1;
    terrain = reg_new_elem(&world->terrain_reg);
    if (!terrain || !terrain_init(terrain, name, asset_id))
        return -1;
    return reg_size(&world->terrain_reg) - 1;
}

ssize_t world_get_terrain(world_t *world, const char *name)
{
    terrain_t *terrain = NULL;

    if (!world || !name)
        return -1;
    for (size_t i = 0; i < reg_size(&world->terrain_reg); i++) {
        terrain = vec_fast_at(&world->terrain_reg.vec, i);
        if (terrain && STR_EQ(terrain->name, name))
            return i;
    }
    return -1;
}

terrain_t *world_get_terrain_ptr(world_t *world, size_t terrain_id)
{
    if (!world || (ssize_t)terrain_id < 0
        || terrain_id >= reg_size(&world->terrain_reg))
        return NULL;
    return vec_fast_at(&world->terrain_reg.vec, terrain_id);
}

///////////////////////////////////////////////////////////////////////////////

#ifdef TEST
#include <criterion/criterion.h>

Test(world, new_terrain)
{
    world_t world = {0};
    ssize_t terrain_id = -1;
    ssize_t asset_id = -1;

    cr_assert(world_init(&world, 10, 10));
    asset_id = world_new_asset(&world, "asset", "path");
    cr_assert_neq(asset_id, -1);
    terrain_id = world_new_terrain(&world, "name", asset_id);
    cr_assert_neq(terrain_id, -1);
    cr_assert_str_eq(world_get_terrain_ptr(&world, terrain_id)->name, "name");
    cr_assert_eq(world_get_terrain_ptr(&world, terrain_id)->asset_id, asset_id);
    world_deinit(&world);
}

Test(world, duplicate_terrain)
{
    world_t world = {0};
    ssize_t terrain_id = -1;
    ssize_t asset_id = -1;

    cr_assert(world_init(&world, 10, 10));
    asset_id = world_new_asset(&world, "asset", "path");
    cr_assert_neq(asset_id, -1);
    terrain_id = world_new_terrain(&world, "name", asset_id);
    cr_assert_neq(terrain_id, -1);
    terrain_id = world_new_terrain(&world, "name", asset_id);
    cr_assert_eq(terrain_id, -1);
    world_deinit(&world);
}

Test(world, invalid_asset_id)
{
    world_t world = {0};
    ssize_t terrain_id = -1;

    cr_assert(world_init(&world, 10, 10));
    terrain_id = world_new_terrain(&world, "name", 0);
    cr_assert_eq(terrain_id, -1, "Invalid asset id should return -1 but got %ld", terrain_id);
    world_deinit(&world);
}

Test(world, get_terrains)
{
    world_t world = {0};
    ssize_t terrain_id = -1;
    ssize_t asset_id = -1;

    cr_assert(world_init(&world, 10, 10));
    asset_id = world_new_asset(&world, "asset", "path");
    cr_assert_neq(asset_id, -1);
    terrain_id = world_new_terrain(&world, "name", asset_id);
    cr_assert_neq(terrain_id, -1);
    cr_assert_eq(world_get_terrain(&world, "name"), terrain_id);
    world_deinit(&world);
}

Test(world, get_terrain_ptr)
{
    world_t world = {0};
    terrain_t *terrain = NULL;
    ssize_t asset_id = -1;

    cr_assert(world_init(&world, 10, 10));
    asset_id = world_new_asset(&world, "asset", "path");
    cr_assert_neq(asset_id, -1);
    world_new_terrain(&world, "name", asset_id);
    terrain = world_get_terrain_ptr(&world, 0);
    cr_assert_not_null(terrain);
    cr_assert_str_eq(terrain->name, "name");
    cr_assert_eq(terrain->asset_id, asset_id);
    world_deinit(&world);
}

#endif