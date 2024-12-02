#include "csp.h"

bool csp_set_in_corner(csp_object_t *obj)
{
    if (!obj || obj->is_collection)
        return false;
    return csp_get_constraint(obj, C_IN_CORNER, true);
}