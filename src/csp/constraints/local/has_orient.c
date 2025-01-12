#include "csp.h"
#include "orientation.h"
#include "utils.h"

static bool validate(UNUSED csp_map_t *map, csp_constraint_t *constraint, UNUSED v2_t pos, UNUSED unsigned int layer, orient_t orient)
{
    return reg_get_if(&constraint->orientations, (comparator_t)orient_eq, &orient);
}

bool csp_set_has_orient(csp_object_t *obj, orient_t orient)
{
    csp_constraint_t *constraint = csp_get_constraint(obj, C_HAS_ORIENT, false);

    if (!constraint) {
        constraint = csp_add_constraint(obj, C_HAS_ORIENT);
        if (!constraint || !reg_init(&constraint->orientations, sizeof(orient_t), 4))
            return false;
        constraint->validate = validate;
    } else if (reg_get_if(&constraint->orientations, (comparator_t)orient_eq, &orient)) {
        return true;
    }
    return reg_push_back(&constraint->orientations, &orient);
}

///////////////////////////////////////////////////////////////////////////////

#ifdef TEST
#include <criterion/criterion.h>

Test(csp_constraint, has_orient)
{
    csp_object_t obj = {0};
    prop_t prop = {0};
    csp_constraint_t *constraint = NULL;

    cr_assert(csp_obj_init(&obj, &prop));
    cr_assert_eq(REG_SIZE(obj.constraints), 0);
    cr_assert(csp_set_has_orient(&obj, ORIENT_DOWN));
    cr_assert_eq(REG_SIZE(obj.constraints), 1);

    constraint = REG_AT(csp_constraint_t, &obj.constraints, 0);
    cr_assert_not_null(constraint);
    cr_assert_eq(constraint->type, C_HAS_ORIENT);
    cr_assert_eq(constraint->orientations.vec.size, 4);
    cr_assert_eq(constraint->orientations.last_free_index, 1);
    csp_obj_deinit(&obj);
}

Test(csp_constraint, has_orient_validation)
{
    csp_map_t map = {0};
    csp_constraint_t *constraint = NULL;
    csp_object_t obj = {0};
    prop_t prop = {0};

    cr_assert(csp_obj_init(&obj, &prop));
    cr_assert(csp_map_init(&map, (v2_t){10, 10}, 3));
    cr_assert(csp_set_has_orient(&obj, ORIENT_DOWN));
    cr_assert(csp_set_has_orient(&obj, ORIENT_LEFT));
    constraint = csp_get_constraint(&obj, C_HAS_ORIENT, false);
    cr_assert_not_null(constraint);
    cr_assert_not_null(constraint->validate);

    cr_assert(constraint->validate(&map, constraint, (v2_t){0, 0}, 0, ORIENT_DOWN));
    cr_assert(constraint->validate(&map, constraint, (v2_t){9, 0}, 1, ORIENT_DOWN));
    cr_assert(constraint->validate(&map, constraint, (v2_t){0, 9}, 2, ORIENT_LEFT));
    cr_assert(constraint->validate(&map, constraint, (v2_t){9, 9}, 1, ORIENT_LEFT));

    cr_assert_not(constraint->validate(&map, constraint, (v2_t){0, 1}, 2, ORIENT_UP));
    cr_assert_not(constraint->validate(&map, constraint, (v2_t){8, 0}, 1, ORIENT_UP));
    cr_assert_not(constraint->validate(&map, constraint, (v2_t){9, 7}, 0, ORIENT_RIGHT));
    cr_assert_not(constraint->validate(&map, constraint, (v2_t){2, 6}, 0, ORIENT_RIGHT));

    cr_assert(csp_set_has_orient(&obj, ORIENT_RIGHT));

    cr_assert(constraint->validate(&map, constraint, (v2_t){9, 7}, 0, ORIENT_RIGHT));
    cr_assert(constraint->validate(&map, constraint, (v2_t){2, 6}, 0, ORIENT_RIGHT));

    csp_obj_deinit(&obj);
    csp_map_deinit(&map);
}

#endif