#include "csp.h"
#include "prop.h"
#include "registry.h"

csp_constraint_t *csp_add_constraint(csp_object_t *obj, csp_constraint_type_t type)
{
    csp_constraint_t *constraint = NULL;

    if (!obj)
        return NULL;
    constraint = reg_new_elem(&obj->constraints);
    if (!constraint)
        return NULL;
    constraint->type = type;
    return constraint;   
}

csp_constraint_t *csp_get_constraint(csp_object_t *obj, csp_constraint_type_t type, bool add_if_absent)
{
    csp_constraint_t *constraint = NULL;

    if (!obj)
        return false;
    for (unsigned int i = 0; i < REG_SIZE(obj->constraints); i++) {
        constraint = REG_AT(csp_constraint_t, &obj->constraints, i);
        if (constraint->type == type)
            return constraint;
    }
    if (add_if_absent)
        return csp_add_constraint(obj, type);
    return NULL;
}


void csp_constraint_deinit(csp_constraint_t *constraint)
{
    if (constraint && constraint->type == C_RESERVED_SPACE)
        reg_deinit(&constraint->positions);
}

///////////////////////////////////////////////////////////////////////////////

#ifdef TEST
#include <criterion/criterion.h>

Test(csp_constraint, add)
{
    csp_object_t obj = {0};
    prop_t prop = {0};
    csp_constraint_t *constraint = NULL;
    csp_constraint_type_t type = C_ADJACENT_TO_WALL;

    cr_assert(csp_obj_init(&obj, &prop));
    cr_assert_eq(REG_SIZE(obj.constraints), 0);
    cr_assert_null(constraint);
    constraint = csp_add_constraint(&obj, type);
    cr_assert_not_null(constraint);
    cr_assert_eq(REG_SIZE(obj.constraints), 1);
    cr_assert_eq(constraint->type, type);
    
    csp_obj_deinit(&obj);
}

Test(csp_constraint, get)
{
    csp_object_t obj = {0};
    prop_t prop = {0};
    csp_constraint_t *constraint = NULL;
    csp_constraint_t *constraint2 = NULL;
    csp_constraint_type_t type = C_ADJACENT_TO_WALL;

    cr_assert(csp_obj_init(&obj, &prop));
    cr_assert_eq(REG_SIZE(obj.constraints), 0);
    cr_assert_null(constraint);
    constraint = csp_add_constraint(&obj, type);
    cr_assert_not_null(constraint);
    cr_assert_eq(REG_SIZE(obj.constraints), 1);
    cr_assert_eq(constraint->type, type);

    constraint2 = csp_get_constraint(&obj, type, false);
    cr_assert_not_null(constraint2);
    cr_assert_eq(constraint, constraint2);
    
    csp_obj_deinit(&obj);
}

Test(csp_constraint, enforce_get)
{
    csp_object_t obj = {0};
    prop_t prop = {0};
    csp_constraint_t *constraint = NULL;
    csp_constraint_type_t type = C_ADJACENT_TO_WALL;

    cr_assert(csp_obj_init(&obj, &prop));
    cr_assert_eq(REG_SIZE(obj.constraints), 0);
    cr_assert_null(constraint);

    constraint = csp_get_constraint(&obj, type, true);
    cr_assert_not_null(constraint);
    cr_assert_eq(REG_SIZE(obj.constraints), 1);
    cr_assert_eq(constraint->type, type);
    
    csp_obj_deinit(&obj);
}

Test(csp_constraint, bad_get)
{
    csp_object_t obj = {0};
    prop_t prop = {0};
    csp_constraint_t *constraint = NULL;
    csp_constraint_type_t type = C_ADJACENT_TO_WALL;
    csp_constraint_type_t not_type = C_ADJACENT_TO_PROP;

    cr_assert(csp_obj_init(&obj, &prop));
    cr_assert_eq(REG_SIZE(obj.constraints), 0);
    cr_assert_null(constraint);
    constraint = csp_add_constraint(&obj, type);
    cr_assert_not_null(constraint);
    cr_assert_eq(REG_SIZE(obj.constraints), 1);
    cr_assert_eq(constraint->type, type);

    cr_assert_null(csp_get_constraint(&obj, not_type, false));

    csp_obj_deinit(&obj);
}

#endif