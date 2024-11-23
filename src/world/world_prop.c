#include "prop.h"
#include "registry.h"
#include "world.h"

ssize_t world_new_prop(world_t *world, const char *name)
{
    prop_t *prop = NULL;

    if (!world || !name || world_get_prop(world, name) != -1)
        return -1;
    prop = reg_new_elem(&world->prop_reg);
    if (!prop || !prop_init(prop, name))
        return -1;
    return reg_size(&world->prop_reg) - 1;
}

ssize_t world_get_prop(world_t *world, const char *name)
{
    prop_t *prop = NULL;

    if (!world || !name)
        return -1;
    for (size_t i = 0; i < reg_size(&world->prop_reg); i++) {
        prop = vec_fast_at(&world->prop_reg.vec, i);
        if (prop && STR_EQ(prop->name, name))
            return i;
    }
    return -1;
}

prop_t *world_get_prop_ptr(world_t *world, size_t prop_id)
{
    if (!world || prop_id > reg_last_idx(&world->prop_reg))
        return NULL;
    return vec_fast_at(&world->prop_reg.vec, prop_id);
}

///////////////////////////////////////////////////////////////////////////////

#ifdef TEST
#include <criterion/criterion.h>

Test(world, new_prop)
{
    world_t world = {0};
    ssize_t prop_id = -1;

    cr_assert(world_init(&world, 10, 10));
    prop_id = world_new_prop(&world, "name");
    cr_assert_neq(prop_id, -1);
    cr_assert_str_eq(world_get_prop_ptr(&world, prop_id)->name, "name");
    world_deinit(&world);
}

Test(world, duplicate_prop)
{
    world_t world = {0};
    ssize_t prop_id = -1;

    cr_assert(world_init(&world, 10, 10));
    prop_id = world_new_prop(&world, "name");
    cr_assert_neq(prop_id, -1);
    prop_id = world_new_prop(&world, "name");
    cr_assert_eq(prop_id, -1);
    world_deinit(&world);
}

Test(world, get_props)
{
    world_t world = {0};
    ssize_t prop_id = -1;

    cr_assert(world_init(&world, 10, 10));
    prop_id = world_new_prop(&world, "name");
    cr_assert_neq(prop_id, -1);
    cr_assert_eq(world_get_prop(&world, "name"), prop_id);
    world_deinit(&world);
}

#endif