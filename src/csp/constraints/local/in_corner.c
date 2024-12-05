#include "csp.h"

bool csp_set_in_corner(csp_object_t *obj)
{
    if (!obj || obj->is_collection)
        return false;
    return csp_get_constraint(obj, C_IN_CORNER, true);
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

#endif