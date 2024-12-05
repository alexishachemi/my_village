#include "csp.h"

bool csp_set_adjacent_to_wall(csp_object_t *obj)
{
    if (!obj || obj->is_collection)
        return false;
    return csp_get_constraint(obj, C_ADJACENT_TO_WALL, true);
}

///////////////////////////////////////////////////////////////////////////////

#ifdef TEST
#include <criterion/criterion.h>

Test(csp_constraint, adjacent_to_wall)
{
    csp_object_t obj = {0};
    prop_t prop = {0};
    csp_constraint_t *constraint = NULL;

    cr_assert(csp_obj_init(&obj, &prop));
    cr_assert_eq(REG_SIZE(obj.constraints), 0);
    cr_assert(csp_set_adjacent_to_wall(&obj));
    cr_assert_eq(REG_SIZE(obj.constraints), 1);

    constraint = REG_AT(csp_constraint_t, &obj.constraints, 0);
    cr_assert_not_null(constraint);
    cr_assert_eq(constraint->type, C_ADJACENT_TO_WALL);
    csp_obj_deinit(&obj);
}

#endif