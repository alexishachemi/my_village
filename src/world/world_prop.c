#include "cvec.h"
#include "linked.h"
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
    prop = reg_new_elem(&world->props);
    if (!prop || !prop_init(prop, name))
        return NULL;
    return prop;
}

prop_t *world_get_prop(world_t *world, const char *name)
{
    prop_t *prop = NULL;

    if (!world || !name)
        return NULL;
    for (size_t i = 0; i < REG_SIZE(world->props); i++) {
        prop = vec_fast_at(&world->props.vec, i);
        if (prop && STR_EQ(prop->name, name))
            return prop;
    }
    return NULL;
}

static void remove_children(world_t *world, v2_t pos, prop_t *prop, orient_t orient)
{
    prop_t *child = NULL;
    v2_t child_pos = {0};

    if (!prop->has_child)
        return;
    for (unsigned int i = 0; i < REG_SIZE(prop->children); i++) {
        child = REG_AT(prop_t, &prop->children, i);
        child_pos = V2_ADD(pos, v2_orient(child->offset, orient));
        tile_remove_prop(world_get_tile(world, child_pos), child);
    }
}

void world_remove_prop(world_t *world, v2_t pos)
{
    tile_t *tile = world_get_tile(world, pos);
    placed_prop_t *placed = NULL;

    if (!tile || !tile->props.head)
        return;
    for (unsigned int i = 0; i < tile->props.size && tile->props.head; i++) {
        placed = list_at(&tile->props, i);
        if (placed->prop->type == PTYPE_PARENT) {
            remove_children(world, pos, placed->prop, placed->orient);
            list_remove_free(&tile->props, i--);
        } else if (placed->prop->type == PTYPE_CHILD) {
            world_remove_prop(world, V2_SUB(pos, v2_orient(placed->prop->offset, placed->orient)));
        }
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