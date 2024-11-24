#include "biome.h"
#include "asset.h"
#include "registry.h"
#include "str.h"
#include "terrain.h"
#include "prop.h"
#include <sys/types.h>

bool biome_init(biome_t *biome, const char *name)
{
    if (!biome || !name)
        return false;
    namecpy(biome->name, name);
    reg_init(&biome->parents, sizeof(biome_t*), BIOME_HERITAGE_BASE_SIZE);
    reg_init(&biome->terrains, sizeof(terrain_t *), BIOME_TERRAIN_BASE_SIZE);
    reg_init(&biome->props, sizeof(prop_t*), BIOME_PROPS_BASE_SIZE);
    return true;
}

void biome_deinit(biome_t *biome)
{
    if (!biome)
        return;
    reg_deinit(&biome->parents);
    reg_deinit(&biome->terrains);
    reg_deinit(&biome->props);
}

bool biome_add_terrain(biome_t *biome, terrain_t *terrain)
{
    if (!biome || !terrain)
        return false;
    return REG_ADD(&biome->terrains, &terrain, sizeof(terrain_t*));
}

bool biome_add_prop(biome_t *biome, prop_t *prop)
{
    if (!biome || !prop)
        return false;
    return REG_ADD(&biome->props, &prop, sizeof(prop_t*));
}

bool biome_add_parent(biome_t *biome, biome_t *parent)
{
    if (!biome || !parent)
        return false;
    return REG_ADD(&biome->parents, &parent, sizeof(biome_t*));
}

///////////////////////////////////////////////////////////////////////////////

#ifdef TEST
#include <criterion/criterion.h>
#include "world.h"

Test(biome, init)
{
    biome_t biome = {0};

    cr_assert(biome_init(&biome, "test_biome"));
    biome_deinit(&biome);
}

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
