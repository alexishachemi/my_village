#include "csp.h"

bool csp_set_adjacent_to_prop(csp_object_t *obj, prop_t *prop)
{
    csp_constraint_t *constraint = NULL;

    if (!obj || !prop || obj->is_collection)
        return false;
    constraint = csp_add_constraint(obj, C_ADJACENT_TO_PROP);
    if (!constraint)
        return false;
    constraint->prop = prop;
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

#endif