#include "cvec.h"
#include "world.h"

ssize_t world_new_asset(world_t *world, const char *name, const char *path)
{
    asset_t *asset = NULL;

    if (!world || !name || !path || world_get_asset(world, name) != -1)
        return -1;
    asset = reg_new_elem(&world->asset_reg);
    if (!asset || !asset_init(asset, name, path))
        return -1;
    return world->asset_reg.last_free_index - 1;
}

ssize_t world_get_asset(world_t *world, const char *name)
{
    asset_t *asset = NULL;

    if (!world || !name)
        return -1;
    for (size_t i = 0; i < reg_size(&world->asset_reg); i++) {
        asset = vec_fast_at(&world->asset_reg.vec, i);
        if (asset && STR_EQ(asset->name, name))
            return i;
    }
    return -1;
}

char *world_get_asset_path(world_t *world, const char *name)
{
    ssize_t asset_id = world_get_asset(world, name);

    if (asset_id < 0)
        return NULL;
    return ((asset_t *)vec_fast_at(&world->asset_reg.vec, asset_id))->path;
}

asset_t *world_get_asset_ptr(world_t *world, size_t asset_id)
{
    if (!world || asset_id >= reg_size(&world->asset_reg))
        return NULL;
    return vec_fast_at(&world->asset_reg.vec, asset_id);
}

///////////////////////////////////////////////////////////////////////////////

#ifdef TEST
#include <criterion/criterion.h>

Test(world, new_asset)
{
    world_t world = {0};
    ssize_t asset_id = -1;

    cr_assert(world_init(&world));
    asset_id = world_new_asset(&world, "name", "path");
    cr_assert_neq(asset_id, -1);
    cr_assert_str_eq(world_get_asset_path(&world, "name"), "path");
    world_deinit(&world);
}

Test(world, duplicate_asset)
{
    world_t world = {0};
    ssize_t asset_id = -1;

    cr_assert(world_init(&world));
    asset_id = world_new_asset(&world, "name", "path");
    cr_assert_neq(asset_id, -1);
    asset_id = world_new_asset(&world, "name", "path");
    cr_assert_eq(asset_id, -1);
    world_deinit(&world);
}

Test(world, get_assets)
{
    world_t world = {0};
    ssize_t asset_id = -1;

    cr_assert(world_init(&world));
    asset_id = world_new_asset(&world, "name", "path");
    cr_assert_neq(asset_id, -1);
    asset_id = world_get_asset(&world, "name");
    cr_assert_neq(asset_id, -1);
    cr_assert_str_eq(world_get_asset_path(&world, "name"), "path");
    world_deinit(&world);
}

Test(world, get_asset_ptr)
{
    world_t world = {0};
    asset_t *asset = NULL;

    cr_assert(world_init(&world));
    world_new_asset(&world, "name", "path");
    asset = world_get_asset_ptr(&world, 0);
    cr_assert_not_null(asset);
    cr_assert_str_eq(asset->name, "name");
    cr_assert_str_eq(asset->path, "path");
    world_deinit(&world);
}

#endif