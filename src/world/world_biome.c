#include "biome.h"
#include "registry.h"
#include "str.h"
#include "world.h"

ssize_t world_add_biome(world_t *world, const char *name)
{
    biome_t *biome = NULL;

    if (!world || !name || world_get_biome(world, name) != -1)
        return -1;
    biome = reg_new_elem(&world->biome_reg);
    if (!biome)
        return -1;
    biome_init(biome, name);
    return reg_last_idx(&world->biome_reg);
}

ssize_t world_get_biome(world_t *world, const char *name)
{
    if (!world || !name)
        return -1;
    for (size_t i = 0; i < reg_size(&world->biome_reg); i++) {
        if (STR_EQ(REG_AT(biome_t, &world->biome_reg, i)->name, name))
            return i;
    }
    return -1;
}

biome_t *world_get_biome_ptr(world_t *world, size_t biome_id)
{
    if (!world || biome_id > reg_last_idx(&world->biome_reg))
        return NULL;
    return REG_AT(biome_t, &world->biome_reg, biome_id);
}

///////////////////////////////////////////////////////////////////////////////

#ifdef TEST
#include <criterion/criterion.h>

Test(world, add_biome)
{
    world_t world = {0};
    ssize_t biome_id = -1;

    cr_assert(world_init(&world, 10, 10));
    biome_id = world_add_biome(&world, "test_biome");
    cr_assert_neq(biome_id, -1);
    cr_assert_eq(world_get_biome(&world, "test_biome"), biome_id);
    world_deinit(&world);
}

Test(world, get_biome_)
{
    world_t world = {0};
    ssize_t biome_id = -1;

    cr_assert(world_init(&world, 10, 10));
    biome_id = world_add_biome(&world, "test_biome");
    cr_assert(biome_id >= 0);
    cr_assert_eq(world_get_biome(&world, "test_biome"), biome_id);
    cr_assert_eq(world_get_biome(&world, "unknown_biome"), -1);
    world_deinit(&world);
}

Test(world, get_biome_ptr_)
{
    world_t world = {0};
    ssize_t biome_id = -1;
    biome_t *biome = NULL;

    cr_assert(world_init(&world, 10, 10));
    biome_id = world_add_biome(&world, "test_biome");
    cr_assert(biome_id >= 0);
    biome = world_get_biome_ptr(&world, biome_id);
    cr_assert(biome);
    cr_assert_eq(STR_EQ(biome->name, "test_biome"), true);
    world_deinit(&world);
}

#endif