#include "csp.h"

bool csp_add_amount_range(csp_object_t *obj, unsigned int min, unsigned int max)
{
    csp_constraint_t *constraint = NULL;

    if (!obj || obj->is_collection || min > max)
        return false;
    constraint = csp_get_constraint(obj, C_AMOUNT_RANGE);
    if (!constraint)
        constraint = csp_add_constraint(obj, C_AMOUNT_RANGE);
    if (!constraint)
        return false;
    constraint->range[0] = min;
    constraint->range[1] = max;
    return true;
}

bool csp_add_amount(csp_object_t *obj, unsigned int nb)
{
    return csp_add_amount_range(obj, nb, nb);
}