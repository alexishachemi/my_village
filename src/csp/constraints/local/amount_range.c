#include "csp.h"

bool csp_set_amount_range(csp_object_t *obj, unsigned int min, unsigned int max)
{
    csp_constraint_t *constraint = NULL;

    if (!obj || min > max)
        return false;
    constraint = csp_get_constraint(obj, C_AMOUNT_RANGE, true);
    if (!constraint)
        return false;
    constraint->range[0] = min;
    constraint->range[1] = max;
    return true;
}

bool csp_set_amount(csp_object_t *obj, unsigned int nb)
{
    return csp_set_amount_range(obj, nb, nb);
}

///////////////////////////////////////////////////////////////////////////////

#ifdef TEST
#include <criterion/criterion.h>

Test(csp_constraint, amount)
{
    csp_object_t obj = {0};
    csp_constraint_t *constraint = NULL;

    cr_assert(csp_obj_init(&obj));
    cr_assert_eq(REG_SIZE(obj.constraints), 0);
    cr_assert(csp_set_amount(&obj, 1));
    cr_assert_eq(REG_SIZE(obj.constraints), 1);

    constraint = REG_AT(csp_constraint_t, &obj.constraints, 0);
    cr_assert_not_null(constraint);
    cr_assert_eq(constraint->type, C_AMOUNT_RANGE);
    cr_assert_eq(constraint->range[0], 1);
    cr_assert_eq(constraint->range[1], 1);

    cr_assert(csp_set_amount(&obj, 3));
    cr_assert_eq(REG_SIZE(obj.constraints), 1);
    cr_assert_eq(REG_AT(csp_constraint_t, &obj.constraints, 0), constraint);
    cr_assert_eq(constraint->range[0], 3);
    cr_assert_eq(constraint->range[1], 3);

    csp_obj_deinit(&obj);
}

Test(csp_constraint, amount_range)
{
    csp_object_t obj = {0};
    csp_constraint_t *constraint = NULL;

    cr_assert(csp_obj_init(&obj));
    cr_assert_eq(REG_SIZE(obj.constraints), 0);
    cr_assert(csp_set_amount_range(&obj, 0, 1));
    cr_assert_eq(REG_SIZE(obj.constraints), 1);

    constraint = REG_AT(csp_constraint_t, &obj.constraints, 0);
    cr_assert_not_null(constraint);
    cr_assert_eq(constraint->type, C_AMOUNT_RANGE);
    cr_assert_eq(constraint->range[0], 0);
    cr_assert_eq(constraint->range[1], 1);

    cr_assert(csp_set_amount_range(&obj, 3, 4));
    cr_assert_eq(REG_SIZE(obj.constraints), 1);
    cr_assert_eq(REG_AT(csp_constraint_t, &obj.constraints, 0), constraint);
    cr_assert_eq(constraint->range[0], 3);
    cr_assert_eq(constraint->range[1], 4);

    csp_obj_deinit(&obj);
}

#endif