#include "biome.h"

bool biome_add_terrain(biome_t *biome, terrain_t *terrain)
{
    if (!biome || !terrain)
        return false;
    return reg_push_back(&biome->terrains, &terrain);
}

bool biome_add_prop(biome_t *biome, prop_t *prop)
{
    if (!biome || !prop)
        return false;
    return reg_push_back(&biome->props, &prop);
}

bool biome_add_parent(biome_t *biome, biome_t *parent)
{
    if (!biome || !parent)
        return false;
    return reg_push_back(&biome->parents, &parent);
}

#ifdef TEST
#include <criterion/criterion.h>
#include "world.h"

Test(biome, add_terrain)
{
    asset_t *asset = NULL;
    terrain_t *terrain = NULL;
    world_t world = {0};
    biome_t biome = {0};
    texture_t texture = {0};

    cr_assert(world_init(&world, 10, 10));
    cr_assert(biome_init(&biome, "test_biome"));
    asset = world_new_asset(&world, "test_image", &texture, (Rectangle){0, 0, 0, 0});
    cr_assert_not_null(asset);
    terrain = world_new_terrain(&world, "test_terrain", asset);
    cr_assert(terrain != NULL);
    cr_assert(biome_add_terrain(&biome, terrain));
    biome_deinit(&biome);
    world_deinit(&world);
}

Test(biome, add_prop)
{
    prop_t *prop = NULL;
    world_t world = {0};
    biome_t biome = {0};

    cr_assert(world_init(&world, 10, 10));
    cr_assert(biome_init(&biome, "test_biome"));
    prop = world_new_prop(&world, "test_prop");
    cr_assert(prop != NULL);
    cr_assert(biome_add_prop(&biome, prop));
    biome_deinit(&biome);
    world_deinit(&world);
}

Test(biome, add_parent)
{
    world_t world = {0};
    biome_t *biome = NULL;
    biome_t *parent = NULL;
    biome_t **biome_pp = NULL;

    cr_assert(world_init(&world, 10, 10));
    biome = world_new_biome(&world, "test_biome");
    cr_assert_not_null(biome);
    parent = world_new_biome(&world, "test_parent");
    cr_assert_not_null(parent);
    cr_assert(biome_add_parent(biome, parent));
    cr_assert_eq(REG_SIZE(biome->parents), 1);
    biome_pp = REG_AT(biome_t*, &biome->parents, 0);
    cr_assert_not_null(biome_pp);
    cr_assert_eq(*biome_pp, parent);
    world_deinit(&world);
}

#endif