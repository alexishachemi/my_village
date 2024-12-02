#include "csp.h"

bool csp_set_amount_range(csp_object_t *obj, unsigned int min, unsigned int max)
{
    csp_constraint_t *constraint = NULL;

    if (!obj || obj->is_collection || min > max)
        return false;
    constraint = csp_get_constraint(obj, C_AMOUNT_RANGE, false);
    if (!constraint) {
        constraint = csp_add_constraint(obj, C_AMOUNT_RANGE);
        if (!constraint)
            return false;
        reg_init(&constraint->positions, sizeof(v2_t), CSP_POS_REG_BASE_SIZE);
    }
    constraint->range[0] = min;
    constraint->range[1] = max;
    return true;
}

bool csp_set_amount(csp_object_t *obj, unsigned int nb)
{
    return csp_set_amount_range(obj, nb, nb);
}