#include "csp.h"
#include "orientation.h"
#include "prop.h"
#include "registry.h"
#include "v2.h"
#include "utils.h"

static bool valid_under(prop_t *under, reg_t *expected)
{
    prop_t *current = NULL;
    
    for (unsigned int i = 0; i < expected->last_free_index; i++) {
        current = *REG_AT(prop_t*, expected, i);
        if (!current)
            return false;
        if (current == under || (under->type == PTYPE_CHILD && under->parent == current))
            return true;
    }
    return false;
}

static bool validate(
    csp_map_t *map,
    UNUSED prop_t *prop,
    csp_constraint_t *constraint,
    v2_t pos,
    unsigned int layer,
    UNUSED orient_t orient
)
{
    csp_cell_t *under = NULL;

    if (layer == 0)
        return false;
    under = csp_map_get_cell(map, pos, layer - 1);
    return under && under->occupant && valid_under(under->occupant, &constraint->props);
}

bool csp_set_on_top_of_prop(csp_object_t *obj, prop_t *prop)
{
    csp_constraint_t *constraint = NULL;

    if (!obj || !prop)
        return false;
    constraint = csp_get_constraint(obj, C_ON_TOP_OF_PROP, false);
    if (!constraint) {
        constraint = csp_add_constraint(obj, C_ON_TOP_OF_PROP);
        if (!constraint || !reg_init(&constraint->props, sizeof(prop_t*), CSP_PROP_REG_BASE_SIZE))
            return false;        
        constraint->validate = validate;
    }
    return reg_push_back(&constraint->props, &prop);
}

///////////////////////////////////////////////////////////////////////////////

#ifdef TEST
#include <criterion/criterion.h>

Test(csp_constraint, on_top_prop)
{
    csp_object_t obj = {0};
    prop_t prop = {0};
    csp_constraint_t *constraint = NULL;

    cr_assert(csp_obj_init(&obj));
    cr_assert_eq(REG_SIZE(obj.constraints), 0);
    cr_assert(csp_set_on_top_of_prop(&obj, &prop));
    cr_assert_eq(REG_SIZE(obj.constraints), 1);
    
    constraint = REG_AT(csp_constraint_t, &obj.constraints, 0);
    cr_assert_not_null(constraint);
    cr_assert_eq(constraint->type, C_ON_TOP_OF_PROP);
    cr_assert_eq(REG_SIZE(constraint->props), 1);
    cr_assert_eq(*REG_AT(prop_t*, &constraint->props, 0), &prop);
    csp_obj_deinit(&obj);
}

Test(csp_constraint, on_top_prop_validation)
{
    csp_map_t map = {0};
    csp_object_t obj = {0};
    prop_t prop = {0};
    prop_t under_prop = {0};
    csp_constraint_t *constraint = NULL;
    csp_cell_t *prop_cell = NULL;

    cr_assert(csp_map_init(&map, (v2_t){10, 10}, 3));
    cr_assert(csp_obj_init(&obj));
    cr_assert(csp_set_on_top_of_prop(&obj, &under_prop));

    constraint = REG_AT(csp_constraint_t, &obj.constraints, 0);
    cr_assert_not_null(constraint);
    cr_assert_not_null(constraint->validate);

    prop_cell = csp_map_get_cell(&map, (v2_t){3, 8}, 1);
    cr_assert_not_null(prop_cell);
    prop_cell->occupied = true;
    prop_cell->occupant = &under_prop;

    cr_assert_not(constraint->validate(&map, &prop, constraint, (v2_t){0, 4}, 0, ORIENT_DOWN));
    cr_assert_not(constraint->validate(&map, &prop, constraint, (v2_t){4, 8}, 0, ORIENT_DOWN));
    cr_assert_not(constraint->validate(&map, &prop, constraint, (v2_t){5, 8}, 1, ORIENT_DOWN));
    cr_assert_not(constraint->validate(&map, &prop, constraint, (v2_t){3, 10}, 1, ORIENT_DOWN));
    cr_assert_not(constraint->validate(&map, &prop, constraint, (v2_t){3, 9}, 2, ORIENT_DOWN));
    cr_assert_not(constraint->validate(&map, &prop, constraint, (v2_t){3, 7}, 2, ORIENT_DOWN));
    cr_assert_not(constraint->validate(&map, &prop, constraint, (v2_t){3, 8}, 0, ORIENT_DOWN));
    cr_assert_not(constraint->validate(&map, &prop, constraint, (v2_t){3, 8}, 1, ORIENT_DOWN));

    cr_assert(constraint->validate(&map, &prop, constraint, (v2_t){3, 8}, 2, ORIENT_DOWN));

    csp_obj_deinit(&obj);
    csp_map_deinit(&map);
}

#endif