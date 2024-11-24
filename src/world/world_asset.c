#include "cvec.h"
#include "world.h"

asset_t *world_new_asset(world_t *world, const char *name,
    texture_t *texture, Rectangle draw_rect)
{
    asset_t *asset = NULL;

    if (!world || !name || world_get_asset(world, name))
        return NULL;
    asset = reg_new_elem(&world->asset_reg);
    if (!asset || !asset_init(asset, name, texture, draw_rect))
        return NULL;
    return asset;
}

asset_t *world_get_asset(world_t *world, const char *name)
{
    asset_t *asset = NULL;

    if (!world || !name)
        return NULL;
    for (size_t i = 0; i < REG_SIZE(world->asset_reg); i++) {
        asset = vec_fast_at(&world->asset_reg.vec, i);
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
    texture_t texture = {0};
    Rectangle draw_rect = {0};

    cr_assert(world_init(&world, 10, 10));
    asset = world_new_asset(&world, "name", &texture, draw_rect);
    cr_assert_not_null(asset);
    cr_assert_str_eq(asset->name, "name");
    world_deinit(&world);
}

Test(world, duplicate_asset)
{
    world_t world = {0};
    asset_t *asset = NULL;
    texture_t texture = {0};
    Rectangle draw_rect = {0};

    cr_assert(world_init(&world, 10, 10));
    asset = world_new_asset(&world, "name", &texture, draw_rect);
    cr_assert_not_null(asset);
    asset = world_new_asset(&world, "name", &texture, draw_rect);
    cr_assert_null(asset);
    world_deinit(&world);
}

Test(world, get_asset)
{
    world_t world = {0};
    asset_t *asset = NULL;
    texture_t texture = {0};
    Rectangle draw_rect = {0};

    cr_assert(world_init(&world, 10, 10));
    asset = world_new_asset(&world, "name", &texture, draw_rect);
    cr_assert_not_null(asset);
    asset = world_get_asset(&world, "name");
    cr_assert_not_null(asset);
    cr_assert_str_eq(asset->name, "name");
    world_deinit(&world);
}

#endif