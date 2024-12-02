#include "csp.h"

bool csp_add_adjacent_to_wall(csp_object_t *obj)
{
    csp_constraint_t *constraint = NULL;

    if (!obj || obj->is_collection)
        return false;
    if (csp_get_constraint(obj, C_ADJACENT_TO_WALL))
        return true;
    constraint = csp_add_constraint(obj);
    if (!constraint)
        return false;
    constraint->type = C_ADJACENT_TO_WALL;
    return true;
}