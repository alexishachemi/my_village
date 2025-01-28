#include "csp.h"
#include "prop.h"
#include "registry.h"

csp_constraint_t *csp_add_constraint(csp_object_t *obj, csp_constraint_type_t type,
    bool expected_match)
{
    csp_constraint_t *constraint = NULL;

    if (!obj)
        return NULL;
    constraint = reg_new_elem(&obj->constraints);
    if (!constraint)
        return NULL;
    constraint->type = type;
    constraint->validate = NULL;
    constraint->expected = expected_match;
    return constraint;
}

csp_constraint_t *csp_get_constraint(csp_object_t *obj, csp_constraint_type_t type,
    bool add_if_absent, bool expected_match)
{
    csp_constraint_t *constraint = NULL;

    if (!obj)
        return false;
    for (unsigned int i = 0; i < REG_SIZE(obj->constraints); i++) {
        constraint = REG_AT(csp_constraint_t, &obj->constraints, i);
        if (constraint->type == type && constraint->expected == expected_match)
            return constraint;
    }
    if (add_if_absent)
        return csp_add_constraint(obj, type, expected_match);
    return NULL;
}

csp_constraint_t *csp_get_props_constraint(csp_object_t *obj, csp_constraint_type_t type,
    bool add_if_absent, bool expected_match)
{
    csp_constraint_t *constraint = csp_get_constraint(obj, type, false, expected_match);

    if (constraint || (!constraint && !add_if_absent))
        return constraint;
    constraint = csp_add_constraint(obj, type, expected_match);
    if (!constraint)
        return NULL;
    if (!reg_init(&constraint->props, sizeof(prop_t *), CSP_PROP_REG_BASE_SIZE))
        NULL;
    return constraint;
}

bool csp_constraint_has_prop(csp_constraint_t *constraint, prop_t *prop)
{
    if (!constraint || !prop)
        return false;
    REG_FOREACH(&constraint->props, prop_t*, curr, {
        if (*curr == prop)
            return true;
    });
    return false;
}

void csp_constraint_deinit(csp_constraint_t *constraint)
{
    if (!constraint)
        return;
    switch (constraint->type) {
        case C_RESERVED_SPACE: reg_deinit(&constraint->positions); break;
        case C_HAS_ORIENT: reg_deinit(&constraint->orientations); break;
        case C_ON_TOP_OF_PROP: reg_deinit(&constraint->props); break;
        default: break;
    }
}

///////////////////////////////////////////////////////////////////////////////

#ifdef TEST
#include <criterion/criterion.h>

Test(csp_constraint, add)
{
    csp_object_t obj = {0};
    csp_constraint_t *constraint = NULL;
    csp_constraint_type_t type = C_ADJACENT_TO_WALL;

    cr_assert(csp_obj_init(&obj));
    cr_assert_eq(REG_SIZE(obj.constraints), 0);
    cr_assert_null(constraint);
    constraint = csp_add_constraint(&obj, type, true);
    cr_assert_not_null(constraint);
    cr_assert_eq(REG_SIZE(obj.constraints), 1);
    cr_assert_eq(constraint->type, type);
    cr_assert(constraint->expected);
    
    csp_obj_deinit(&obj);
}

Test(csp_constraint, get)
{
    csp_object_t obj = {0};
    csp_constraint_t *constraint = NULL;
    csp_constraint_t *constraint2 = NULL;
    csp_constraint_type_t type = C_ADJACENT_TO_WALL;

    cr_assert(csp_obj_init(&obj));
    cr_assert_eq(REG_SIZE(obj.constraints), 0);
    cr_assert_null(constraint);
    constraint = csp_add_constraint(&obj, type, true);
    cr_assert_not_null(constraint);
    cr_assert_eq(REG_SIZE(obj.constraints), 1);
    cr_assert_eq(constraint->type, type);

    constraint2 = csp_get_constraint(&obj, type, false, true);
    cr_assert_not_null(constraint2);
    cr_assert_eq(constraint, constraint2);
    
    csp_obj_deinit(&obj);
}

Test(csp_constraint, enforce_get)
{
    csp_object_t obj = {0};
    csp_constraint_t *constraint = NULL;
    csp_constraint_type_t type = C_ADJACENT_TO_WALL;

    cr_assert(csp_obj_init(&obj));
    cr_assert_eq(REG_SIZE(obj.constraints), 0);
    cr_assert_null(constraint);

    constraint = csp_get_constraint(&obj, type, true, true);
    cr_assert_not_null(constraint);
    cr_assert_eq(REG_SIZE(obj.constraints), 1);
    cr_assert_eq(constraint->type, type);
    
    csp_obj_deinit(&obj);
}

Test(csp_constraint, bad_get)
{
    csp_object_t obj = {0};
    csp_constraint_t *constraint = NULL;
    csp_constraint_type_t type = C_ADJACENT_TO_WALL;
    csp_constraint_type_t not_type = C_ADJACENT_TO_PROP;

    cr_assert(csp_obj_init(&obj));
    cr_assert_eq(REG_SIZE(obj.constraints), 0);
    cr_assert_null(constraint);
    constraint = csp_add_constraint(&obj, type, true);
    cr_assert_not_null(constraint);
    cr_assert_eq(REG_SIZE(obj.constraints), 1);
    cr_assert_eq(constraint->type, type);

    cr_assert_null(csp_get_constraint(&obj, not_type, false, true));

    csp_obj_deinit(&obj);
}

#endif