#include "csp.h"

bool csp_add_in_corner(csp_object_t *obj)
{
    if (!obj || obj->is_collection)
        return false;
    return csp_add_constraint(obj, C_IN_CORNER);
}