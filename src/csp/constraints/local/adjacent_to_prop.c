#include "csp.h"

bool csp_add_adjacent_to_prop(csp_object_t *obj, prop_t *prop)
{
    csp_constraint_t *constraint = NULL;

    if (!obj || !prop || obj->is_collection)
        return false;
    constraint = csp_add_constraint(obj);
    if (!constraint)
        return false;
    constraint->type = C_ADJACENT_TO_WALL;
    constraint->prop = prop;
    return true;
}