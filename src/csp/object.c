#include "csp.h"
#include "registry.h"

bool csp_obj_init(csp_object_t *obj, prop_t *prop)
{
    if (!obj || !prop)
        return false;
    obj->is_collection = false;
    obj->prop = prop;
    return reg_init(&obj->constraints, sizeof(csp_constraint_t), CSP_CONSTRAINT_SIZE);
}

void csp_obj_deinit(csp_object_t *obj)
{
    if (!obj || obj->is_collection)
        return;
    reg_map(&obj->constraints, (reg_callback_t)csp_constraint_deinit);
    reg_deinit(&obj->constraints);
}

///////////////////////////////////////////////////////////////////////////////

#ifdef TEST
#include <criterion/criterion.h>

Test(csp_obj, init)
{
    csp_object_t obj = {0};
    prop_t prop = {0};

    cr_assert(csp_obj_init(&obj, &prop));
    cr_assert_eq(obj.constraints.vec.size, CSP_CONSTRAINT_SIZE);
    cr_assert_eq(REG_SIZE(obj.constraints), 0);
    cr_assert_not(obj.is_collection);
    cr_assert_eq(obj.prop, &prop);
    csp_obj_deinit(&obj);
}

#endif