#include "biome.h"
#include "registry.h"
#include "str.h"
#include "world.h"

biome_t *world_new_biome(world_t *world, const char *name)
{
    biome_t *biome = NULL;

    if (!world || !name || world_get_biome(world, name))
        return NULL;
    biome = reg_new_elem(&world->biome_reg);
    if (!biome)
        return NULL;
    biome_init(biome, name);
    return biome;
}

biome_t *world_get_biome(world_t *world, const char *name)
{
    biome_t *biome = NULL;

    if (!world || !name)
        return NULL;
    for (size_t i = 0; i < REG_SIZE(world->biome_reg); i++) {
        biome = REG_AT(biome_t, &world->biome_reg, i);
        if (biome && STR_EQ(biome->name, name))
            return biome;
    }
    return NULL;
}

///////////////////////////////////////////////////////////////////////////////

#ifdef TEST
#include <criterion/criterion.h>

Test(world, add_biome)
{
    world_t world = {0};
    biome_t *biome = NULL;

    cr_assert(world_init(&world, 10, 10));
    biome = world_new_biome(&world, "test_biome");
    cr_assert_not_null(biome);
    cr_assert_eq(world_get_biome(&world, "test_biome"), biome);
    world_deinit(&world);
}

Test(world, get_biome)
{
    world_t world = {0};
    biome_t *biome = NULL;

    cr_assert(world_init(&world, 10, 10));
    biome = world_new_biome(&world, "test_biome");
    cr_assert_not_null(biome);
    cr_assert_eq(world_get_biome(&world, "test_biome"), biome);
    cr_assert_null(world_get_biome(&world, "unknown_biome"));
    world_deinit(&world);
}

#endif
