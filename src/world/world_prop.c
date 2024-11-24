#include "prop.h"
#include "registry.h"
#include "world.h"

prop_t *world_new_prop(world_t *world, const char *name)
{
    prop_t *prop = NULL;

    if (!world || !name || world_get_prop(world, name))
        return NULL;
    prop = reg_new_elem(&world->prop_reg);
    if (!prop || !prop_init(prop, name))
        return NULL;
    return prop;
}

prop_t *world_get_prop(world_t *world, const char *name)
{
    prop_t *prop = NULL;

    if (!world || !name)
        return NULL;
    for (size_t i = 0; i < REG_SIZE(world->prop_reg); i++) {
        prop = vec_fast_at(&world->prop_reg.vec, i);
        if (prop && STR_EQ(prop->name, name))
            return prop;
    }
    return NULL;
}

///////////////////////////////////////////////////////////////////////////////

#ifdef TEST
#include <criterion/criterion.h>

Test(world, new_prop)
{
    world_t world = {0};
    prop_t *prop = NULL;

    cr_assert(world_init(&world, 10, 10));
    prop = world_new_prop(&world, "name");
    cr_assert_not_null(prop);
    cr_assert_str_eq(prop->name, "name");
    world_deinit(&world);
}

Test(world, duplicate_prop)
{
    world_t world = {0};
    prop_t *prop = NULL;

    cr_assert(world_init(&world, 10, 10));
    prop = world_new_prop(&world, "name");
    cr_assert_not_null(prop);
    prop = world_new_prop(&world, "name");
    cr_assert_null(prop);
    world_deinit(&world);
}

Test(world, get_prop)
{
    world_t world = {0};
    prop_t *prop = NULL;

    cr_assert(world_init(&world, 10, 10));
    prop = world_new_prop(&world, "name");
    cr_assert_not_null(prop);
    cr_assert_eq(world_get_prop(&world, "name"), prop);
    world_deinit(&world);
}

#endif