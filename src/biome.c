#include "biome.h"
#include "cvec.h"
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
    reg_init(&biome->heritage, sizeof(size_t), BIOME_HERITAGE_BASE_SIZE);
    reg_init(&biome->terrains, sizeof(size_t), BIOME_TERRAIN_BASE_SIZE);
    reg_init(&biome->props, sizeof(size_t), BIOME_PROPS_BASE_SIZE);
    return true;
}

void biome_deinit(biome_t *biome)
{
    if (!biome)
        return;
    reg_deinit(&biome->heritage);
    reg_deinit(&biome->terrains);
    reg_deinit(&biome->props);
}

bool biome_add_terrain(biome_t *biome, size_t terrain_id)
{
    size_t *id = NULL;

    if (!biome)
        return false;
    id = reg_new_elem(&biome->terrains);
    if (!id)
        return false;
    *id = terrain_id;
    return true;
}

bool biome_add_terrain_by_name(biome_t *biome, reg_t *terrains, const char *name)
{
    ssize_t terrain_id = -1;

    if (!biome || !terrains || !name)
        return false;
    for (size_t i = 0; i < terrains->last_free_index; i++) {
        if (STR_EQ(VEC_FAST_AT(terrain_t, &terrains->vec, i).name, name)) {
            terrain_id = i;
            break;
        }
    }
    return terrain_id >= 0 && biome_add_terrain(biome, terrain_id);
}

bool biome_add_prop(biome_t *biome, size_t prop_id)
{
    size_t *id = NULL;

    if (!biome)
        return false;
    id = reg_new_elem(&biome->props);
    if (!id)
        return false;
    *id = prop_id;
    return true;
}

bool biome_add_prop_by_name(biome_t *biome, reg_t *props, const char *name)
{
    ssize_t prop_id = -1;

    if (!biome || !props || !name)
        return false;
    for (size_t i = 0; i < props->last_free_index; i++) {
        if (STR_EQ(VEC_FAST_AT(prop_t, &props->vec, i).name, name)) {
            prop_id = i;
            break;
        }
    }
    return prop_id >= 0 && biome_add_prop(biome, prop_id);
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
    ssize_t asset_id = -1;
    ssize_t terrain_id = -1;
    world_t world = {0};
    biome_t biome = {0};

    cr_assert(world_init(&world));
    cr_assert(biome_init(&biome, "test_biome"));
    asset_id = world_new_asset(&world, "test_image", "path/to/image");
    cr_assert(asset_id >= 0);
    terrain_id = world_new_terrain(&world, "test_terrain", asset_id);
    cr_assert(terrain_id >= 0);
    cr_assert(biome_add_terrain(&biome, terrain_id));
    biome_deinit(&biome);
    world_deinit(&world);
}

Test(biome, add_terrain_by_name)
{
    ssize_t asset_id = -1;
    ssize_t terrain_id = -1;
    world_t world = {0};
    biome_t biome = {0};

    cr_assert(world_init(&world));
    cr_assert(biome_init(&biome, "test_biome"));
    asset_id = world_new_asset(&world, "test_image", "path/to/image");
    cr_assert(asset_id >= 0);
    terrain_id = world_new_terrain(&world, "test_terrain", asset_id);
    cr_assert(terrain_id >= 0);
    cr_assert(biome_add_terrain_by_name(&biome, &world.terrain_reg, "test_terrain"));
    biome_deinit(&biome);
    world_deinit(&world);
}

Test(biome, add_prop)
{
    ssize_t prop_id = -1;
    world_t world = {0};
    biome_t biome = {0};

    cr_assert(world_init(&world));
    cr_assert(biome_init(&biome, "test_biome"));
    prop_id = world_new_prop(&world, "test_prop");
    cr_assert(prop_id >= 0);
    cr_assert(biome_add_prop(&biome, prop_id));
    biome_deinit(&biome);
    world_deinit(&world);
}

Test(biome, add_prop_by_name)
{
    ssize_t prop_id = -1;
    world_t world = {0};
    biome_t biome = {0};

    cr_assert(world_init(&world));
    cr_assert(biome_init(&biome, "test_biome"));
    prop_id = world_new_prop(&world, "test_prop");
    cr_assert(prop_id >= 0);
    cr_assert(biome_add_prop_by_name(&biome, &world.prop_reg, "test_prop"));
    biome_deinit(&biome);
    world_deinit(&world);
}

#endif