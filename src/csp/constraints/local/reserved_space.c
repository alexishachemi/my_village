#include "csp.h"
#include "registry.h"
#include "v2.h"

static bool validate(csp_map_t *map, csp_constraint_t *constraint, v2_t pos, unsigned int layer)
{
    csp_cell_t *cell = NULL;
    v2_t offset = {0};

    cell = csp_map_get_cell(map, pos, layer);
    if (!cell || cell->occupied) {
        return false;
    }
    for (unsigned int i = 0; i < REG_SIZE(constraint->positions); i++) {
        offset = *REG_AT(v2_t, &constraint->positions, i);
        cell = csp_map_get_cell(map, V2_ADD(pos, offset), layer);
        if (!cell || cell->occupied) {
            return false;
        }
    }
    return true;
}

bool csp_set_reserved_space(csp_object_t *obj, v2_t position)
{
    csp_constraint_t *constraint = NULL;

    if (!obj || obj->is_collection)
        return false;
    constraint = csp_get_constraint(obj, C_RESERVED_SPACE, false);
    if (!constraint) {
        constraint = csp_add_constraint(obj, C_RESERVED_SPACE);
        if (!constraint || !reg_init(&constraint->positions, sizeof(v2_t), CSP_POS_REG_BASE_SIZE))
            return false;
        constraint->validate = validate;
    }
    return REG_ADD(&constraint->positions, &position, sizeof(v2_t));
}

///////////////////////////////////////////////////////////////////////////////

#ifdef TEST
#include <criterion/criterion.h>

Test(csp_constraint, reserved_space)
{
    csp_object_t obj = {0};
    prop_t prop = {0};
    csp_constraint_t *constraint = NULL;
    v2_t *pos = NULL;

    cr_assert(csp_obj_init(&obj, &prop));
    cr_assert_eq(REG_SIZE(obj.constraints), 0);
    cr_assert(csp_set_reserved_space(&obj, (v2_t){1, 0}));
    cr_assert_eq(REG_SIZE(obj.constraints), 1);

    constraint = REG_AT(csp_constraint_t, &obj.constraints, 0);
    cr_assert_not_null(constraint);
    cr_assert_eq(constraint->type, C_RESERVED_SPACE);
    cr_assert_eq(REG_SIZE(constraint->positions), 1);
    
    pos = REG_AT(v2_t, &constraint->positions, 0);
    cr_assert_not_null(pos);
    cr_assert_eq(pos->x, 1);
    cr_assert_eq(pos->y, 0);
    
    cr_assert(csp_set_reserved_space(&obj, (v2_t){3, 1}));
    cr_assert_eq(REG_SIZE(obj.constraints), 1);
    cr_assert_eq(REG_AT(csp_constraint_t, &obj.constraints, 0), constraint);
    cr_assert_eq(REG_AT(v2_t, &constraint->positions, 0), pos);
    cr_assert_eq(pos->x, 1);
    cr_assert_eq(pos->y, 0);

    pos = REG_AT(v2_t, &constraint->positions, 1);
    cr_assert_not_null(pos);
    cr_assert_eq(pos->x, 3);
    cr_assert_eq(pos->y, 1);

    csp_obj_deinit(&obj);
}

static void occupy_cell(csp_map_t *map, v2_t pos, unsigned int layer)
{
    csp_cell_t *cell = csp_map_get_cell(map, pos, layer);
    
    cr_assert_not_null(cell);
    cell->occupied = true;
}

Test(csp_constraint, reserved_space_validation)
{
    csp_map_t map = {0};
    csp_object_t obj = {0};
    prop_t prop = {0};
    csp_constraint_t *constraint = NULL;

    cr_assert(csp_map_init(&map, (v2_t){10, 10}, 3));
    cr_assert(csp_obj_init(&obj, &prop));

    cr_assert(csp_set_reserved_space(&obj, (v2_t){0, -1})); //  #
    cr_assert(csp_set_reserved_space(&obj, (v2_t){-1, 0})); // #O 
    cr_assert(csp_set_reserved_space(&obj, (v2_t){0, 1}));  //  #

    occupy_cell(&map, (v2_t){3, 2}, 0);
    occupy_cell(&map, (v2_t){5, 5}, 1);
    occupy_cell(&map, (v2_t){1, 1}, 2);

    constraint = csp_get_constraint(&obj, C_RESERVED_SPACE, false);
    cr_assert_not_null(constraint);
    cr_assert_not_null(constraint->validate);

    cr_assert_not(constraint->validate(&map, constraint, (v2_t){0, 0}, 0));
    cr_assert_not(constraint->validate(&map, constraint, (v2_t){9, 0}, 2));
    cr_assert_not(constraint->validate(&map, constraint, (v2_t){1, 1}, 2));
    cr_assert_not(constraint->validate(&map, constraint, (v2_t){3, 3}, 0));
    cr_assert_not(constraint->validate(&map, constraint, (v2_t){6, 5}, 1));
    cr_assert_not(constraint->validate(&map, constraint, (v2_t){9, 9}, 0));

    cr_assert(constraint->validate(&map, constraint, (v2_t){1, 1}, 0));
    cr_assert(constraint->validate(&map, constraint, (v2_t){3, 3}, 1));
    cr_assert(constraint->validate(&map, constraint, (v2_t){6, 5}, 2));
    cr_assert(constraint->validate(&map, constraint, (v2_t){4, 5}, 1));
    cr_assert(constraint->validate(&map, constraint, (v2_t){9, 8}, 0));
}

#endif