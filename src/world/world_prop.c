#include "cvec.h"
#include "orientation.h"
#include "prop.h"
#include "registry.h"
#include "tile.h"
#include "v2.h"
#include "world.h"
#include <time.h>

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


static void remove_childs(world_t *world, v2_t pos, prop_t *prop, orient_t orient)
{
    tile_t *tile = NULL;
    prop_t *child = NULL;
    v2_t child_pos = {0};

    if (!prop->has_child)
        return;
    for (unsigned int i = 0; i < REG_SIZE(prop->childs); i++) {
        child = REG_AT(prop_t, &prop->childs, i);
        child_pos = V2_ADD(pos, v2_orient(child->offset, orient));
        tile = world_get_tile(world, child_pos);
        if (tile)
            tile->prop = NULL;
    }
}

void world_remove_prop(world_t *world, v2_t pos)
{
    tile_t *tile = world_get_tile(world, pos);

    if (!tile || !tile->prop)
        return;
    if (tile->prop->type == PTYPE_PARENT) {
        remove_childs(world, pos, tile->prop, tile->prop_orient);
        tile->prop = NULL;
    } else if (tile->prop->type == PTYPE_CHILD) {
        world_remove_prop(world, V2_SUB(pos, v2_orient(tile->prop->offset, tile->prop_orient)));
    }
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