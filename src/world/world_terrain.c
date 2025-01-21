#include "str.h"
#include "terrain.h"
#include "world.h"

terrain_t *world_new_terrain(world_t *world, const char *name, asset_t *asset)
{
    terrain_t *terrain = NULL;

    if (!world || !name || !asset || world_get_terrain(world, name))
        return NULL;
    terrain = reg_new_elem(&world->terrains);
    if (!terrain || !terrain_init(terrain, name, asset))
        return NULL;
    return terrain;
}

terrain_t *world_get_terrain(world_t *world, const char *name)
{
    terrain_t *terrain = NULL;

    if (!world || !name)
        return NULL;
    for (size_t i = 0; i < REG_SIZE(world->terrains); i++) {
        terrain = REG_AT(terrain_t, &world->terrains, i);
        if (terrain && STR_EQ(terrain->name, name))
            return terrain;
    }
    return NULL;
}

#ifdef TEST
#include <criterion/criterion.h>

Test(world, new_terrain)
{
    world_t world = {0};
    terrain_t *terrain = NULL;
    asset_t *asset = NULL;
    texture_t texture = {0};

    cr_assert(world_init(&world, 10, 10));
    asset = world_new_asset(&world, "asset", &texture, (Rectangle){0, 0, 0, 0});
    cr_assert_not_null(asset);
    terrain = world_new_terrain(&world, "name", asset);
    cr_assert_not_null(terrain);
    cr_assert_str_eq(terrain->name, "name");
    cr_assert_eq(terrain->asset, asset);
    world_deinit(&world);
}

Test(world, duplicate_terrain)
{
    world_t world = {0};
    terrain_t *terrain = NULL;
    asset_t *asset = NULL;
    texture_t texture = {0};

    cr_assert(world_init(&world, 10, 10));
    asset = world_new_asset(&world, "asset", &texture, (Rectangle){0, 0, 0, 0});
    cr_assert_not_null(asset);
    terrain = world_new_terrain(&world, "name", asset);
    cr_assert_not_null(terrain);
    terrain = world_new_terrain(&world, "name", asset);
    cr_assert_null(terrain);
    world_deinit(&world);
}

Test(world, invalid_asset)
{
    world_t world = {0};
    terrain_t *terrain = NULL;

    cr_assert(world_init(&world, 10, 10));
    terrain = world_new_terrain(&world, "name", NULL);
    cr_assert_null(terrain);
    world_deinit(&world);
}

Test(world, get_terrain)
{
    world_t world = {0};
    terrain_t *terrain = NULL;
    asset_t *asset = NULL;
    texture_t texture = {0};

    cr_assert(world_init(&world, 10, 10));
    asset = world_new_asset(&world, "asset", &texture, (Rectangle){0, 0, 0, 0});
    cr_assert_not_null(asset);
    terrain = world_new_terrain(&world, "name", asset);
    cr_assert_not_null(terrain);
    cr_assert_eq(world_get_terrain(&world, "name"), terrain);
    world_deinit(&world);
}

Test(world, get_terrain_bad)
{
    world_t world = {0};

    cr_assert(world_init(&world, 10, 10));
    cr_assert_null(world_get_terrain(&world, "name"));
    world_deinit(&world);
}

#endif
