#include "csp.h"

bool csp_set_reserved_space(csp_object_t *obj, v2_t position)
{
    csp_constraint_t *constraint = NULL;

    if (!obj || obj->is_collection || position.x < 0 || position.y < 0)
        return false;
    constraint = csp_get_constraint(obj, C_RESERVED_SPACE, false);
    if (!constraint) {
        constraint = csp_add_constraint(obj, C_RESERVED_SPACE);
        if (!constraint || !reg_init(&constraint->positions, sizeof(v2_t), CSP_POS_REG_BASE_SIZE))
            return false;
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

#endif