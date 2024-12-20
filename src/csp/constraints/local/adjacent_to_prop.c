#include "csp.h"
#include "orientation.h"
#include "prop.h"
#include "v2.h"
#include "utils.h"

static bool validate(csp_map_t *map, csp_constraint_t *constraint, v2_t pos, unsigned int layer, UNUSED orient_t orient)
{
    const v2_t directions[] = { {1, 0}, {-1, 0}, {0, 1}, {0, -1} };
    csp_cell_t *cell = NULL;
    prop_t *prop = constraint->prop;

    for (unsigned int i = 0; i < 4; i++) {
        cell = csp_map_get_cell(map, V2_ADD(pos, directions[i]), layer);
        if (cell && cell->occupant == prop)
            return true;
    }
    return false;
}

bool csp_set_adjacent_to_prop(csp_object_t *obj, prop_t *prop)
{
    csp_constraint_t *constraint = NULL;

    if (!obj || !prop || obj->is_collection)
        return false;
    constraint = csp_add_constraint(obj, C_ADJACENT_TO_PROP);
    if (!constraint)
        return false;
    constraint->prop = prop;
    constraint->validate = validate;
    return true;
}

///////////////////////////////////////////////////////////////////////////////

#ifdef TEST
#include <criterion/criterion.h>

Test(csp_constraint, adjacent_to_prop)
{
    csp_object_t obj = {0};
    prop_t prop = {0};
    csp_constraint_t *constraint = NULL;

    cr_assert(csp_obj_init(&obj, &prop));
    cr_assert_eq(REG_SIZE(obj.constraints), 0);
    cr_assert(csp_set_adjacent_to_prop(&obj, &prop));
    cr_assert_eq(REG_SIZE(obj.constraints), 1);
    
    constraint = REG_AT(csp_constraint_t, &obj.constraints, 0);
    cr_assert_not_null(constraint);
    cr_assert_eq(constraint->type, C_ADJACENT_TO_PROP);
    cr_assert_eq(constraint->prop, &prop);
    csp_obj_deinit(&obj);
}

Test(csp_constraint, adjacent_to_prop_validation)
{
    csp_map_t map = {0};
    csp_object_t obj = {0};
    prop_t prop = {0};
    prop_t adj_prop = {0};
    csp_constraint_t *constraint = NULL;
    csp_cell_t *prop_cell = NULL;

    cr_assert(csp_map_init(&map, (v2_t){10, 10}, 3));
    cr_assert(csp_obj_init(&obj, &prop));
    cr_assert(csp_set_adjacent_to_prop(&obj, &adj_prop));

    constraint = REG_AT(csp_constraint_t, &obj.constraints, 0);
    cr_assert_not_null(constraint);
    cr_assert_not_null(constraint->validate);

    prop_cell = csp_map_get_cell(&map, (v2_t){3, 8}, 1);
    cr_assert_not_null(prop_cell);
    prop_cell->occupied = true;
    prop_cell->occupant = &adj_prop;

    cr_assert_not(constraint->validate(&map, constraint, (v2_t){0, 4}, 0, ORIENT_DOWN));
    cr_assert_not(constraint->validate(&map, constraint, (v2_t){4, 8}, 0, ORIENT_DOWN));
    cr_assert_not(constraint->validate(&map, constraint, (v2_t){5, 8}, 1, ORIENT_DOWN));
    cr_assert_not(constraint->validate(&map, constraint, (v2_t){3, 10}, 1, ORIENT_DOWN));
    cr_assert_not(constraint->validate(&map, constraint, (v2_t){3, 9}, 2, ORIENT_DOWN));
    cr_assert_not(constraint->validate(&map, constraint, (v2_t){3, 7}, 2, ORIENT_DOWN));

    cr_assert(constraint->validate(&map, constraint, (v2_t){4, 8}, 1, ORIENT_DOWN));
    cr_assert(constraint->validate(&map, constraint, (v2_t){2, 8}, 1, ORIENT_DOWN));
    cr_assert(constraint->validate(&map, constraint, (v2_t){3, 9}, 1, ORIENT_DOWN));
    cr_assert(constraint->validate(&map, constraint, (v2_t){3, 7}, 1, ORIENT_DOWN));

    csp_obj_deinit(&obj);
    csp_map_deinit(&map);
}

#endif