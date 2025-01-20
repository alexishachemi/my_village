#include "csp.h"
#include "orientation.h"
#include "utils.h"
#include "v2.h"

static bool validate(
    UNUSED csp_map_t *map,
    UNUSED prop_t *prop,
    UNUSED csp_constraint_t *constraint,
    UNUSED v2_t pos,
    unsigned int layer,
    UNUSED orient_t orient
)
{
    return layer == 0;
}

bool csp_set_on_ground(csp_object_t *obj, bool expected)
{
    csp_constraint_t *constraint = NULL;

    if (!obj)
        return false;
    constraint = csp_get_constraint(obj, C_ON_GROUND, true);
    if (!constraint)
        return NULL;
    constraint->validate = validate;
    constraint->expected = expected;
    return constraint;
}

///////////////////////////////////////////////////////////////////////////////

#ifdef TEST
#include <criterion/criterion.h>

Test(csp_constraint, on_ground)
{
    csp_object_t obj = {0};
    csp_constraint_t *constraint = NULL;

    cr_assert(csp_obj_init(&obj));
    cr_assert_eq(REG_SIZE(obj.constraints), 0);
    cr_assert(csp_set_on_ground(&obj, true));
    cr_assert_eq(REG_SIZE(obj.constraints), 1);

    constraint = REG_AT(csp_constraint_t, &obj.constraints, 0);
    cr_assert_not_null(constraint);
    cr_assert_eq(constraint->type, C_ON_GROUND);
    csp_obj_deinit(&obj);
}

Test(csp_constraint, on_ground_validation)
{
    csp_map_t map = {0};
    csp_constraint_t *constraint = NULL;
    csp_object_t obj = {0};
    prop_t prop = {0};
    csp_room_t room = {0};

    cr_assert(csp_room_init(&room, "foo"));
    cr_assert(csp_obj_init(&obj));
    cr_assert(csp_map_init(&map, &room, (v2_t){10, 10}));
    cr_assert(csp_set_on_ground(&obj, true));
    constraint = csp_get_constraint(&obj, C_ON_GROUND, false);
    cr_assert_not_null(constraint);
    cr_assert_not_null(constraint->validate);

    cr_assert(constraint->validate(&map, &prop, constraint, (v2_t){0, 0}, 0, ORIENT_DOWN));
    cr_assert(constraint->validate(&map, &prop, constraint, (v2_t){9, 0}, 0, ORIENT_DOWN));
    cr_assert(constraint->validate(&map, &prop, constraint, (v2_t){1, 6}, 0, ORIENT_DOWN));
    cr_assert(constraint->validate(&map, &prop, constraint, (v2_t){3, 4}, 0, ORIENT_DOWN));

    cr_assert_not(constraint->validate(&map, &prop, constraint, (v2_t){0, 1}, 2, ORIENT_DOWN));
    cr_assert_not(constraint->validate(&map, &prop, constraint, (v2_t){8, 0}, 1, ORIENT_DOWN));
    cr_assert_not(constraint->validate(&map, &prop, constraint, (v2_t){9, 7}, 1, ORIENT_DOWN));
    cr_assert_not(constraint->validate(&map, &prop, constraint, (v2_t){2, 6}, 1, ORIENT_DOWN));

    csp_obj_deinit(&obj);
    csp_map_deinit(&map);
    csp_room_deinit(&room);
}

#endif