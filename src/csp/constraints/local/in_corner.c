#include "csp.h"
#include "utils.h"
#include "v2.h"

static bool validate(csp_map_t *map, UNUSED csp_constraint_t *constraint, v2_t pos, UNUSED unsigned int layer)
{
    v2_t corners[] = {{0, 0}, {map->size.x - 1, 0}, {0, map->size.y - 1}, {map->size.x - 1, map->size.y - 1}};

    for (unsigned int i = 0; i < 4; i++) {
        if (V2_EQ(pos, corners[i]))
            return true;
    }
    return false;
}

bool csp_set_in_corner(csp_object_t *obj)
{
    csp_constraint_t *constraint = NULL;

    if (!obj || obj->is_collection)
        return false;
    constraint = csp_get_constraint(obj, C_IN_CORNER, true);
    if (!constraint)
        return NULL;
    constraint->validate = validate;
    return constraint;
}

///////////////////////////////////////////////////////////////////////////////

#ifdef TEST
#include <criterion/criterion.h>

Test(csp_constraint, in_corner)
{
    csp_object_t obj = {0};
    prop_t prop = {0};
    csp_constraint_t *constraint = NULL;

    cr_assert(csp_obj_init(&obj, &prop));
    cr_assert_eq(REG_SIZE(obj.constraints), 0);
    cr_assert(csp_set_in_corner(&obj));
    cr_assert_eq(REG_SIZE(obj.constraints), 1);

    constraint = REG_AT(csp_constraint_t, &obj.constraints, 0);
    cr_assert_not_null(constraint);
    cr_assert_eq(constraint->type, C_IN_CORNER);
    csp_obj_deinit(&obj);
}

Test(csp_constraint, in_corner_validation)
{
    csp_map_t map = {0};
    csp_constraint_t *constraint = NULL;
    csp_object_t obj = {0};
    prop_t prop = {0};

    cr_assert(csp_obj_init(&obj, &prop));
    cr_assert(csp_map_init(&map, (v2_t){10, 10}, 3));
    cr_assert(csp_set_in_corner(&obj));
    constraint = csp_get_constraint(&obj, C_IN_CORNER, false);
    cr_assert_not_null(constraint);
    cr_assert_not_null(constraint->validate);

    cr_assert(constraint->validate(&map, constraint, (v2_t){0, 0}, 0));
    cr_assert(constraint->validate(&map, constraint, (v2_t){9, 0}, 1));
    cr_assert(constraint->validate(&map, constraint, (v2_t){0, 9}, 2));
    cr_assert(constraint->validate(&map, constraint, (v2_t){9, 9}, 1));

    cr_assert_not(constraint->validate(&map, constraint, (v2_t){0, 1}, 2));
    cr_assert_not(constraint->validate(&map, constraint, (v2_t){8, 0}, 1));
    cr_assert_not(constraint->validate(&map, constraint, (v2_t){9, 7}, 0));
    cr_assert_not(constraint->validate(&map, constraint, (v2_t){2, 6}, 0));

    csp_obj_deinit(&obj);
    csp_map_deinit(&map);
}

#endif