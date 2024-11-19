#include "cvec.h"
#include "world.h"

asset_t *world_new_asset(world_t *world, const char *name, const char *path)
{
    asset_t *asset = NULL;

    if (!world || !name || !path || world_get_asset(world, name))
        return NULL;
    asset = reg_new_elem(&world->asset_reg);
    if (!asset || !asset_init(asset, name, path))
        return NULL;
    return asset;
}

asset_t *world_get_asset(world_t *world, const char *name)
{
    asset_t *asset = NULL;

    if (!world || !name)
        return NULL;
    for (size_t i = 0; i < reg_size(&world->asset_reg); i++) {
        asset = (asset_t *)vec_fast_at(&world->asset_reg.vec, i);
        if (asset && STR_EQ(asset->name, name))
            return asset;
    }
    return NULL;
}

///////////////////////////////////////////////////////////////////////////////

#ifdef TEST
#include <criterion/criterion.h>

Test(world, new_asset)
{
    world_t world = {0};
    asset_t *asset = NULL;

    cr_assert(world_init(&world));
    asset = world_new_asset(&world, "name", "path");
    cr_assert_not_null(asset);
    cr_assert_str_eq(asset->name, "name");
    cr_assert_str_eq(asset->path, "path");
    world_deinit(&world);
}

Test(world, duplicate_asset)
{
    world_t world = {0};
    asset_t *asset = NULL;

    cr_assert(world_init(&world));
    asset = world_new_asset(&world, "name", "path");
    cr_assert_not_null(asset);
    asset = world_new_asset(&world, "name", "path");
    cr_assert_null(asset);
    world_deinit(&world);
}

Test(world, get_asset)
{
    world_t world = {0};
    asset_t *asset = NULL;

    cr_assert(world_init(&world));
    asset = world_new_asset(&world, "name", "path");
    cr_assert_not_null(asset);
    asset = world_get_asset(&world, "name");
    cr_assert_not_null(asset);
    cr_assert_str_eq(asset->name, "name");
    cr_assert_str_eq(asset->path, "path");
    world_deinit(&world);
}

#endif