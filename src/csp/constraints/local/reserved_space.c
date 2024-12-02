#include "csp.h"

bool csp_set_reserved_space(csp_object_t *obj, v2_t position)
{
    csp_constraint_t *constraint = NULL;

    if (!obj || obj->is_collection || position.x < 0 || position.y < 0)
        return false;
    constraint = csp_get_constraint(obj, C_RESERVED_SPACE, true);
    if (!constraint)
        return false;
    return REG_ADD(&constraint->positions, &position, sizeof(v2_t));
}