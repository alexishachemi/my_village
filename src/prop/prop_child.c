#include "prop.h"
#include "registry.h"
#include "str.h"
#include "v2.h"
#include <stdio.h>
#include <sys/types.h>

bool prop_has_child(const prop_t *prop)
{
    return prop->type == PTYPE_PARENT && prop->has_child;
}

static bool init_child_reg(prop_t *prop)
{
    prop->has_child = 
        reg_init(&prop->childs, sizeof(prop_t), PROP_CHILD_REGISTRY_BASE_SIZE);
    return prop->has_child;
}

static prop_t *new_child(prop_t *prop)
{
    if (prop->type != PTYPE_PARENT
        || (!prop->has_child && !init_child_reg(prop)))
        return NULL;
    return reg_new_elem(&prop->childs);   
}

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wformat-truncation"
static const char *generate_child_name(const prop_t *parent, char *buf,
    v2_t offset)
{
    snprintf(buf, NAME_SIZE, PROP_CHILD_NAME_FORMAT,
        parent->name, offset.x, offset.y);
    return buf;
}
#pragma GCC diagnostic pop

static bool offset_valid(v2_t offset)
{
    return offset.x > 0 && offset.y > 0;
}

prop_t *prop_add_child(prop_t *prop, const prop_asset_map_t *map, v2_t offset)
{
    name_t child_name = {0};
    prop_t *child = NULL;

    if (!prop || !map || !offset_valid(offset) || prop_get_child(prop, offset))
        return NULL;
    child = new_child(prop);
    if (!prop_init(child, generate_child_name(prop, child_name, offset)))
        return NULL;
    child->type = PTYPE_CHILD;
    child->parent = prop;
    child->offset = offset;
    return child;
}

prop_t *prop_get_child(prop_t *prop, v2_t offset)
{
    prop_t *child = NULL;

    if (!prop || prop->type != PTYPE_PARENT)
        return NULL;
    if (offset.x == 0 && offset.y == 0) 
        return prop;
    if (!prop->has_child)
        return NULL;
    for (unsigned int i = 0; i < REG_SIZE(prop->childs); i++) {
        child = REG_AT(prop_t, &prop->childs, i);
        if (child && V2_EQ(child->offset, offset))
            return child;
    }
    return NULL;
}

///////////////////////////////////////////////////////////////////////////////

#ifdef TEST
#include <criterion/criterion.h>
#define LONG_NAME "yellow_sofisticated_antique_wall_clock"

Test(prop, add_child)
{
    prop_t prop = {0};
    prop_t *child = NULL;
    prop_t *child2 = NULL;
    prop_asset_map_t map = {0};

    cr_assert(prop_init(&prop, LONG_NAME));
    child = prop_add_child(&prop, &map, (v2_t){1, 1});
    cr_assert_not_null(child);
    cr_assert_null(prop_add_child(&prop, &map, (v2_t){1, 1}));
    cr_assert_eq(REG_SIZE(prop.childs), 1);
    cr_assert_eq(child, REG_AT(prop_t, &prop.childs, 0));
    child2 = prop_add_child(&prop, &map, (v2_t){1, 2});
    cr_assert_not_null(child2);
    cr_assert_eq(REG_SIZE(prop.childs), 2);
    cr_assert_str_eq(child->name, "Child prop of ["LONG_NAME"] at [1,1]");
    cr_assert_eq(child->type, PTYPE_CHILD);
    cr_assert_eq(child->offset.x, 1);
    cr_assert_eq(child->offset.y, 1);
    cr_assert_eq(child->parent, &prop);
    cr_assert_eq(child->asset_map.asset_mode, AMODE_NONE);
}

Test(prop, get_child)
{
    prop_t prop = {0};
    prop_t *child = NULL;
    prop_asset_map_t map = {0};

    cr_assert(prop_init(&prop, "name"));
    cr_assert(prop_add_child(&prop, &map, (v2_t){1, 1}));
    cr_assert_eq(REG_SIZE(prop.childs), 1);
    child = prop_get_child(&prop, (v2_t){1, 1});
    cr_assert(child);
    cr_assert_eq(child->type, PTYPE_CHILD);
    cr_assert_eq(child->offset.x, 1);
    cr_assert_eq(child->offset.y, 1);
    cr_assert_eq(child->parent, &prop);
    cr_assert_eq(child->asset_map.asset_mode, AMODE_NONE);
    cr_assert_eq(prop_get_child(&prop, (v2_t){0, 0}), &prop);
    cr_assert_null(prop_get_child(&prop, (v2_t){1, 0}));
    cr_assert_null(prop_get_child(&prop, (v2_t){0, 1}));
}

#endif