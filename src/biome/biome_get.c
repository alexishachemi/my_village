#include "biome.h"
#include "registry.h"
#include "str.h"

biome_t *biome_get_parent(biome_t *biome, const char *name)
{
    biome_t *curr = NULL;

    if (!biome || !name)
        return NULL;
    for (unsigned int i = 0; i < REG_SIZE(biome->parents); i++) {
        curr = *REG_AT(biome_t*, &biome->parents, i);
        if (STR_EQ(curr->name, name))
            return curr;
    }
    for (unsigned int i = 0; i < REG_SIZE(biome->parents); i++) {
        curr = biome_get_parent(*REG_AT(biome_t*, &biome->parents, i), name);
        if (curr)
            return curr;
    }
    return NULL;
}

terrain_t *biome_get_terrain(biome_t *biome, const char *name)
{
    terrain_t *curr = NULL;

    if (!biome || !name)
        return NULL;
    for (unsigned int i = 0; i < REG_SIZE(biome->terrains); i++) {
        curr = *REG_AT(terrain_t*, &biome->terrains, i);
        if (STR_EQ(curr->name, name))
            return curr;
    }
    for (unsigned int i = 0; i < REG_SIZE(biome->parents); i++) {
        curr = biome_get_terrain(*REG_AT(biome_t*, &biome->parents, i), name);
        if (curr)
            return curr;
    }
    return NULL;
}

prop_t *biome_get_prop(biome_t *biome, const char *name)
{
    prop_t *curr = NULL;

    if (!biome || !name)
        return NULL;
    for (unsigned int i = 0; i < REG_SIZE(biome->props); i++) {
        curr = *REG_AT(prop_t*, &biome->props, i);
        if (STR_EQ(curr->name, name))
            return curr;
    }
    for (unsigned int i = 0; i < REG_SIZE(biome->parents); i++) {
        curr = biome_get_prop(*REG_AT(biome_t*, &biome->parents, i), name);
        if (curr)
            return curr;
    }
    return NULL;
}

#ifdef TEST
#include <criterion/criterion.h>

Test(biome, simple_get)
{
    biome_t biome = {0};
    terrain_t terrain1 = {.name="terrain1"};
    terrain_t terrain2 = {.name="terrain2"};
    prop_t prop1 = {.name="prop1"};
    prop_t prop2 = {.name="prop2"};

    cr_assert(biome_init(&biome, "test_biome"));

    cr_assert(biome_add_terrain(&biome, &terrain1));
    cr_assert(biome_add_terrain(&biome, &terrain2));
    cr_assert(biome_add_prop(&biome, &prop1));
    cr_assert(biome_add_prop(&biome, &prop2));

    cr_assert_eq(biome_get_terrain(&biome, "terrain1"), &terrain1);
    cr_assert_eq(biome_get_terrain(&biome, "terrain2"), &terrain2);
    cr_assert_eq(biome_get_prop(&biome, "prop1"), &prop1);
    cr_assert_eq(biome_get_prop(&biome, "prop2"), &prop2);

    cr_assert_null(biome_get_terrain(&biome, "terrain3"));
    cr_assert_null(biome_get_prop(&biome, "prop3"));

    biome_deinit(&biome);
}

Test(biome, get_with_parent)
{
    biome_t biome = {0};
    biome_t parent = {0};
    terrain_t terrain1 = {.name="terrain1"};
    terrain_t terrain2 = {.name="terrain2"};
    prop_t prop1 = {.name="prop1"};
    prop_t prop2 = {.name="prop2"};

    cr_assert(biome_init(&biome, "test_biome"));
    cr_assert(biome_init(&parent, "test_parent"));

    cr_assert(biome_add_terrain(&biome, &terrain1));
    cr_assert(biome_add_terrain(&parent, &terrain2));
    cr_assert(biome_add_prop(&biome, &prop1));
    cr_assert(biome_add_prop(&parent, &prop2));

    cr_assert_eq(biome_get_terrain(&biome, "terrain1"), &terrain1);
    cr_assert_null(biome_get_terrain(&biome, "terrain2"));
    cr_assert_eq(biome_get_prop(&biome, "prop1"), &prop1);
    cr_assert_null(biome_get_prop(&biome, "prop2"));

    cr_assert(biome_add_parent(&biome, &parent));

    cr_assert_eq(biome_get_terrain(&biome, "terrain1"), &terrain1);
    cr_assert_eq(biome_get_terrain(&biome, "terrain2"), &terrain2);
    cr_assert_eq(biome_get_prop(&biome, "prop1"), &prop1);
    cr_assert_eq(biome_get_prop(&biome, "prop2"), &prop2);

    biome_deinit(&biome);
}

#endif
